#include <htpp.hpp>
#include <client.hpp>
#include <route.hpp>
#include <handler.hpp>
#include <sys/socket.h>
#include <list>
#include <array>
#include <fstream>
#include <iostream>

htpp::htpp::htpp(const htpp_builder &builder) : m_cleaner_semaphore(0)
{
    m_port = builder.port;
    m_max_listen_queue = builder.max_listen_queue;
    m_max_request_size = builder.max_request_size;

    if(std::filesystem::is_directory(builder.docroot))
    {
        m_docroot = builder.docroot;
    }

    m_socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    
    {
        int32_t opt_value = 1;

        setsockopt(m_socket_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt_value, sizeof(opt_value));
    }

    m_address.sin_family = AF_INET;
    m_address.sin_addr.s_addr = INADDR_ANY;
    m_address.sin_port = htons(m_port);   // it needs opposite byte order lol

    bind(m_socket_fd, (sockaddr *)&m_address, sizeof(m_address));
    listen(m_socket_fd, m_max_listen_queue);

    m_route_segment_tree = new route::segment_tree_node();

    if(std::filesystem::exists(m_docroot))
    {
        for(const std::filesystem::directory_entry &fs_entry : std::filesystem::recursive_directory_iterator(m_docroot))
        {
            if(std::filesystem::is_regular_file(fs_entry) && fs_entry.path().filename() == "index.html")
            {
                const std::filesystem::path relative_path = std::filesystem::relative(fs_entry, m_docroot);
                const std::filesystem::path parent_path = relative_path.parent_path();
                std::vector<route::segment> segments;

                {
                    std::list<route::segment> segments_list;

                    for(std::filesystem::path::const_iterator it = parent_path.begin(); it != parent_path.end(); ++it)
                    {
                        route::segment new_segment(it->string());

                        if(new_segment.is_valid())
                        {
                            segments_list.push_back(new_segment);
                        }
                    }

                    segments.resize(segments_list.size());

                    size_t next = 0;

                    for(std::list<route::segment>::const_iterator it = segments_list.cbegin(); it != segments_list.cend(); ++it)
                    {
                        segments[next] = *it;
                        ++next;
                    }
                }

                handler index_handler(segments, [](const request &req) -> response
                {
                    std::filesystem::path index_path(req.get_route().get_path() + "/index.html");
                    std::ifstream index_stream(index_path);
                    std::stringstream ss;

                    ss << index_stream.rdbuf();

                    response index_response;
                    index_response.content_type = "text/html";
                    index_response.body = ss.str();

                    return index_response;
                });

                route::segment_tree_node *bottom = m_route_segment_tree;

                for(const route::segment &segment : segments)
                {
                    const std::string &segment_name = segment.get_name();
                    std::map<std::string, route::segment_tree_node *>::const_iterator found = bottom->children.find(segment_name);

                    if(found == bottom->children.end())
                    {
                        route::segment_tree_node *new_node = new route::segment_tree_node();

                        bottom->children.insert({segment_name, new route::segment_tree_node()});

                        bottom = new_node;
                    }
                    else
                    {
                        bottom = found->second;
                    }
                }

                bottom->set_handler_get(std::move(index_handler));
            }
        }
    }
}

void htpp::htpp::run()
{
    m_cleaner = std::thread([this]() -> void
    {
        while(true)
        {
            m_cleaner_semaphore.acquire();
            m_dead_connecion_mutex.lock();

            if(m_dead_connections.size() > 0)
            {
                client *front = m_dead_connections.front();

                m_dead_connections.pop();

                front->get_thread().join();

                delete front;
            }

            m_dead_connecion_mutex.unlock();
        }
    });

    socklen_t address_len = sizeof(m_address);
    constexpr size_t connector_count = 1;
    std::array<std::thread, connector_count> connectors;

    for(size_t i = 0; i < connector_count; ++i)
    {
        connectors[i] = std::thread([this, &address_len]() -> void
        {
            while(true)
            {
                int32_t client_socket_fd = accept(m_socket_fd, (sockaddr *)&m_address, &address_len);
                client *new_client = new client(*this, client_socket_fd);

                new_client->run();
            }
        });
    }

    for(size_t i = 0; i < connector_count; ++i)
    {
        connectors[i].join();
    }
}

const int32_t &htpp::htpp::get_socket_fd() const
{
    return m_socket_fd;
}

const size_t &htpp::htpp::get_max_request_size() const
{
    return m_max_request_size;
}

const std::filesystem::path &htpp::htpp::get_docroot() const
{
    return m_docroot;
}

void htpp::htpp::enqueue_dead_connection(client *dead_client)
{
    m_dead_connecion_mutex.lock();
    m_dead_connections.push(dead_client);
    m_dead_connecion_mutex.unlock();
    m_cleaner_semaphore.release();
}

htpp::htpp::~htpp()
{
    delete m_route_segment_tree;
}