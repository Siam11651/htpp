#include <client.hpp>
#include <unistd.h>
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
        char buffer[max_request_size] = "";

        read(m_socket_fd, buffer, max_request_size - 1);

        std::cout << buffer << std::endl;

        m_server.enqueue_dead_connection(this);
    });
}

std::thread &htpp::client::get_thread()
{
    return m_handler;
}