#ifndef COOKIE_H
#define COOKIE_H

#include <string>
#include <string_view>
#include <map>
#include <optional>

namespace htpp
{
    class cookies
    {
    public:
        enum class samesite
        {
            strict,
            lax,
            none
        };

        class data
        {
        public:
            std::optional<std::string> value;
            std::optional<std::string> domain;
            std::optional<std::string> expires;
            std::optional<std::string> max_age;
            std::optional<std::string> path;
            std::optional<bool> http_only;
            std::optional<bool> partitioned;
            std::optional<bool> secure;
            std::optional<samesite> samesite_policy;
        };

    private:
        std::map<std::string, data> m_cookies_map;

    public:
        cookies();
        void parse(const std::string_view &value);
        
    };
}

#endif