#ifndef CLIENT_H
#define CLIENT_H

#include <htpp.hpp>
#include <llhttp.h>

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
    };
}

#endif