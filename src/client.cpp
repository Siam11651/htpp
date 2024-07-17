#include <client.hpp>
#include <request.hpp>
#include <unistd.h>
#include <poll.h>
#include <functional>
#include <fstream>
#include <chrono>
#include <iomanip>
#include <iostream>

htpp::client::client(htpp &server, const int32_t &socket_fd) : m_server(server)
{
    m_socket_fd = socket_fd;
}

void htpp::client::run()
{
    m_handler = std::thread([this]() -> void
                            {
        while(true)
        {
            pollfd socket_poll;
            socket_poll.fd = m_socket_fd;
            socket_poll.events = POLLIN;

            if(poll(&socket_poll, 1, 5000) > 0)
            {
                const size_t max_request_size = m_server.get_max_request_size();
                char request_buffer[max_request_size] = "";

                read(m_socket_fd, request_buffer, max_request_size - 1);

                request_buffer[max_request_size - 1] = '\0';
                request http_request(request_buffer);

                if(!http_request.is_valid())
                {
                    continue;
                }

                const std::string url = http_request.get_route().get_path();
                const std::filesystem::path filepath = m_server.get_docroot().string() + url;
                const bool file_request = http_request.get_method() == request::method::GET && std::filesystem::is_regular_file(filepath);

                if(file_request)
                {
                    std::string mime_type = "application/octet-stream";

                    if(filepath.has_extension())
                    {
                        if(filepath.extension() == ".html")
                        {
                            mime_type = "text/html";
                        }
                        else if(filepath.extension() == ".css")
                        {
                            mime_type = "text/css";
                        }
                        else if(filepath.extension() == ".js")
                        {
                            mime_type = "text/javascript";
                        }
                        else if(filepath.extension() == ".webp")
                        {
                            mime_type = "image/webp";
                        }
                    }

                    std::ifstream page_file(filepath);
                    std::string response_data;

                    {
                        std::stringstream ss;

                        ss << page_file.rdbuf();

                        response_data = ss.str();
                    }

                    time_t now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
                    std::stringstream response_stream;

                    response_stream << "HTTP/1.1 200 OK" << std::endl;
                    response_stream << "Date: " << std::put_time(std::gmtime(&now), "%a, %d %b %Y %H:%M:%S GMT") << std::endl;
                    response_stream << "Content-Type: " << mime_type << "; charset=utf-8" << std::endl;
                    response_stream << "Server: htpp" << std::endl;
                    response_stream << "Content-Length: " << response_data.size() << std::endl;
                    response_stream << std::endl;
                    response_stream << response_data << std::endl;

                    std::string response = response_stream.str();

                    send(m_socket_fd, response.c_str(), response.size(), 0);
                }
                else
                {
                    const handler *http_handler = extract_handler(http_request.get_method(), http_request.get_route().get_segments());

                    handle_response(http_request, http_handler);
                }
            }
            else
            {
                break;
            }
        }

        m_server.enqueue_dead_connection(this);
    });
}

std::thread &htpp::client::get_thread()
{
    return m_handler;
}

const htpp::handler *htpp::client::extract_handler(const request::method &method, const std::vector<route::segment> &segments) const
{
    const route::segment_tree_node *bottom = m_server.get_route_segment_tree_ptr();

    for(size_t i = 0; i < segments.size(); ++i)
    {
        std::map<std::string, route::segment_tree_node *>::const_iterator found = bottom->children.find(segments[i].get_name());

        if(found == bottom->children.end())
        {
            std::map<std::string, route::segment_tree_node *>::const_iterator var_found = bottom->children.find("");

            if(var_found == bottom->children.end())
            {
                response response404;
                response404.status_code = 404;
                const std::string serialized_response = response404.serialize();

                send(m_socket_fd, serialized_response.c_str(), serialized_response.size(), 0);
            }
            else
            {
                bottom = var_found->second;
            }
        }
        else
        {
            bottom = found->second;
        }
    }

    if(method == request::method::GET)
    {
        return bottom->get_handler_get_ptr();
    }
    else
    {
        return nullptr;
    }
}

void htpp::client::handle_response(const request &http_request, const handler *request_handler) const
{
    if(request_handler)
    {
        const response http_response = request_handler->handle(http_request);
        const std::string serialized_response = http_response.serialize();

        send(m_socket_fd, serialized_response.c_str(), serialized_response.size(), 0);
    }
    else
    {
        response response405;
        response405.status_code = 405;
        const std::string serialized_response = response405.serialize();

        send(m_socket_fd, serialized_response.c_str(), serialized_response.size(), 0);
    }
}