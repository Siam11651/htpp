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
        std::string_view m_extension_method;
        std::string_view m_http_major;
        std::string_view m_http_minor;
        route m_route;

        bool is_interger(const std::string_view &query) const;

    public:
        request(const std::string_view &message);
    };
}

#endif