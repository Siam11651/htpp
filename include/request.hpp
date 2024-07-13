#ifndef REQUEST_H
#define REQUEST_H

#include <route.hpp>
#include <string>
#include <map>
#include <optional>

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
        std::string m_http_major;
        std::string m_http_minor;
        route m_route;
        std::map<std::string, std::string> m_headers;

        bool is_interger(const std::string &query) const;

    public:
        request(const std::string &message);
        const method &get_method() const;
        const route &get_route() const;
        const bool is_valid() const;
        const std::optional<std::string> get_header(const std::string &name) const;
    };
}

#endif