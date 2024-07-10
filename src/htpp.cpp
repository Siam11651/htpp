#include <htpp.hpp>
#include <client.hpp>
#include <sys/socket.h>
#include <iostream>

htpp::htpp::htpp(const htpp_builder &builder)
{
    m_port = builder.port;
    m_max_listen_queue = builder.max_listen_queue;
    m_max_request_size = builder.max_request_size;
    m_docroot = builder.docroot;
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
}

void htpp::htpp::run()
{
    m_cleaner = std::thread([this]() -> void
    {
        while(true)
        {
            m_dead_connecion_mutex.lock();

            if(m_dead_connections.size() > 0)
            {
                client *front = m_dead_connections.front();

                m_dead_connections.pop();

                front->get_thread().join();

                delete front;
            }

            m_dead_connecion_mutex.unlock();

            std::this_thread::sleep_for(std::chrono::milliseconds(250));    // doing this to avoid race :3
        }
    });

    socklen_t address_len = sizeof(m_address);

    std::clog << "Listening to requests on port " << m_port << "..." << std::endl;

    while(true)
    {
        int32_t client_socket_fd = accept(m_socket_fd, (sockaddr *)&m_address, &address_len);
        client *new_client = new client(*this, client_socket_fd);

        new_client->run();
    }
}

int32_t htpp::htpp::get_socket_fd() const
{
    return m_socket_fd;
}

size_t htpp::htpp::get_max_request_size() const
{
    return m_max_request_size;
}

void htpp::htpp::enqueue_dead_connection(client *dead_client)
{
    m_dead_connecion_mutex.lock();
    m_dead_connections.push(dead_client);
    m_dead_connecion_mutex.unlock();
}