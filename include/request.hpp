#ifndef REQUEST_H
#define REQUEST_H

#include <route.hpp>
#include <string>

namespace htpp
{
    class request
    {
    public:
        enum class method
        {
            OPTIONS,
            GET,
            HEAD,
            POST,
            PUT,
            DELETE,
            TRACE
        };

    private:
        bool m_healthy = true;
        method m_method;
        std::string m_extension_method;
        std::string m_http_major;
        std::string m_http_minor;
        route m_route;

        bool is_interger(const std::string &query) const;

    public:
        request(const std::string &message);
    };
}

#endif