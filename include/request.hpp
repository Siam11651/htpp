#ifndef REQUEST_H
#define REQUEST_H

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
            TRACE,
            CONNECT,
            EXTENSION
        };

    private:
        bool m_healthy = true;
        method m_method;
        std::string_view m_extension_method;
        std::string_view m_http_major;
        std::string_view m_http_minor;

        bool is_interger(const std::string_view &query) const;

    public:
        request(const std::string_view &message);
    };
}

#endif