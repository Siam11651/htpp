#include <client.hpp>
#include <unistd.h>
#include <functional>
#include <fstream>
#include <iostream>

htpp::client::client(htpp &server, const int32_t &socket_fd) : m_server(server)
{
    m_socket_fd = socket_fd;
}

void htpp::client::run()
{
    m_handler = std::thread([this]() -> void
    {
        const size_t max_request_size = m_server.get_max_request_size();
        char request_buffer[max_request_size] = "";
        llhttp_t http_parser;
        llhttp_settings_t http_parser_settings;

        read(m_socket_fd, request_buffer, max_request_size - 1);
        llhttp_settings_init(&http_parser_settings);

        http_parser_settings.on_url = [](llhttp__internal_s *http_parser, const char *at, size_t length) -> int
        {
            std::string *url = new std::string(at, length);
            http_parser->data = url;

            return 0;
        };

        llhttp_init(&http_parser, HTTP_REQUEST, &http_parser_settings);
        llhttp_execute(&http_parser, request_buffer, max_request_size);

        const std::string &url = *(std::string *)http_parser.data;
        std::filesystem::path filepath = m_server.get_docroot().string() + url;
        std::string mime_type = "text/html";

        if(filepath.has_extension())
        {
            if(filepath.extension() == ".js")
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

        std::cout << filepath << " " << mime_type << std::endl;

        std::ifstream page_file(filepath);
        std::string response_data;

        {
            std::stringstream ss;

            ss << page_file.rdbuf();

            response_data = ss.str();
        }

        std::stringstream response_stream;

        response_stream << "HTTP/1.1 200 OK" << std::endl;
        response_stream << "Content-Type: " << mime_type << "; charset=utf-8" << std::endl;
        response_stream << "Server: htpp" << std::endl;
        response_stream << "Content-Length: " << response_data.size() << std::endl;
        response_stream << std::endl;
        response_stream << response_data << std::endl;

        std::string response = response_stream.str();

        send(m_socket_fd, response.c_str(), response.size(), 0);

        m_server.enqueue_dead_connection(this);
    });
}

std::thread &htpp::client::get_thread()
{
    return m_handler;
}