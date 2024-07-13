#ifndef HTPP_H
#define HTPP_H

#include <route-segment-tree.hpp>
#include <netinet/in.h>
#include <cstddef>
#include <cstdint>
#include <thread>
#include <filesystem>
#include <queue>
#include <mutex>
#include <semaphore>

namespace htpp
{
    class htpp_builder
    {
    public:
        uint16_t port = 443;
        size_t max_request_size = 1048576;
        int32_t max_listen_queue = 69420;
        std::filesystem::path docroot;
    };

    class client;

    class htpp
    {
    private:
        uint16_t m_port;
        size_t m_max_request_size;
        int32_t m_max_listen_queue;
        std::filesystem::path m_docroot;
        int32_t m_socket_fd;
        sockaddr_in m_address;
        route::segment_tree_node *m_route_segment_tree;
        std::mutex m_dead_connecion_mutex;
        std::counting_semaphore<0> m_cleaner_semaphore;
        std::queue<client *> m_dead_connections;
        std::thread m_cleaner;

    public:
        htpp(const htpp_builder &builder);
        void run();
        const int32_t &get_socket_fd() const;
        const size_t &get_max_request_size() const;
        const std::filesystem::path &get_docroot() const;
        void enqueue_dead_connection(client *dead_client);
        ~htpp();
    };
}

#endif