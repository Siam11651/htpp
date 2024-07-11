#include <client.hpp>
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
                llhttp_t http_parser;
                llhttp_settings_t http_parser_settings;

                read(m_socket_fd, request_buffer, max_request_size - 1);

                request_buffer[max_request_size - 1] = '\0';

                llhttp_settings_init(&http_parser_settings);

                http_parser_settings.on_url = [](llhttp__internal_s *http_parser, const char *at, size_t length) -> int
                {
                    std::string *url = new std::string(at, length);
                    http_parser->data = url;

                    return 0;
                };

                llhttp_init(&http_parser, HTTP_REQUEST, &http_parser_settings);
                llhttp_execute(&http_parser, request_buffer, max_request_size);

                if(http_parser.data)
                {
                    const std::string &url = *(std::string *)http_parser.data;
                    std::filesystem::path filepath = m_server.get_docroot().string() + url;
                    std::string mime_type = "text/html";

                    if(filepath.has_extension())
                    {
                        if(filepath.extension() == ".css")
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
                    else
                    {
                        filepath = filepath.string() + "/index.html";
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
                    response_stream << "Connection: close" << std::endl;
                    response_stream << "Date: " << std::put_time(std::gmtime(&now), "%a, %d %b %Y %H:%M:%S GMT") << std::endl;
                    response_stream << "Content-Type: " << mime_type << "; charset=utf-8" << std::endl;
                    response_stream << "Server: htpp" << std::endl;
                    response_stream << "Content-Length: " << response_data.size() << std::endl;
                    response_stream << std::endl;
                    response_stream << response_data << std::endl;

                    std::string response = response_stream.str();

                    send(m_socket_fd, response.c_str(), response.size(), 0);

                    delete &url;

                    break;
                }
            }
            else
            {
                break;
            }
        }

        m_server.enqueue_dead_connection(this); });
}

std::thread &htpp::client::get_thread()
{
    return m_handler;
}