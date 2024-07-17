#ifndef CLIENT_H
#define CLIENT_H

#include <htpp.hpp>

namespace htpp
{
    class client
    {
    private:
        htpp &m_server;
        int32_t m_socket_fd;
        bool m_dead = false;
        std::thread m_handler;

    public:
        client(htpp &server, const int32_t &socket_fd);
        void run();
        std::thread &get_thread();
        const handler *extract_handler(const request::method &method, const std::vector<route::segment> &segments) const;
        void handle_response(const request &http_request, const handler *request_handler) const;
    };
}

#endif