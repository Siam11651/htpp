#include <cookies.hpp>
#include <string-view.hpp>
#include <vector>
#include <ranges>
#include <algorithm>

htpp::cookies::cookies()
{
    
}

void htpp::cookies::parse(const std::string_view &header)
{
    std::vector<std::pair<std::string_view, std::string_view>> attributes;

    for(const auto &attribute_sub_range : std::ranges::split_view(header, ";"))
    {
        const string_view attribute(attribute_sub_range.data(), attribute_sub_range.size());
        const size_t equal_position = attribute.find("=");
        const string_view key(attribute.substr(0, equal_position));
        const string_view value(attribute.substr(equal_position + 1));

        attributes.push_back({key, value});
    }

    std::map<std::string, data>::iterator cookie_entry;

    if(!attributes.empty())
    {
        const std::string_view &key_view = attributes.front().first;
        const std::string key(key_view.data(), key_view.size());
        cookie_entry = m_cookies_map.find(key);

        if(cookie_entry == m_cookies_map.end())
        {
            cookie_entry = m_cookies_map.insert({std::move(key), {}}).first;
        }

        const std::string_view &value_view = attributes.front().second;
        cookie_entry->second.value = std::string(value_view.data(), value_view.size());
    }

    for(size_t i = 1; i < attributes.size(); ++i)
    {
        const std::string_view &key_view = attributes[i].first;
        const std::string_view &value_view = attributes[i].second;
        std::string lower_key;

        std::transform(key_view.cbegin(), key_view.cend(), lower_key.end(), [](const char &c) -> char
        {
            return std::tolower(c);
        });

        if(lower_key == "domain")
        {
            cookie_entry->second.domain = std::string(value_view.data(), value_view.size());
        }
        else if(lower_key == "expires")
        {
            cookie_entry->second.expires = std::string(value_view.data(), value_view.size());
        }
        else if(lower_key == "httponly")
        {
            cookie_entry->second.http_only = true;
        }
        else if(lower_key == "max-age")
        {
            cookie_entry->second.max_age = std::string(value_view.data(), value_view.size());
        }
        else if(lower_key == "partitioned")
        {
            cookie_entry->second.partitioned = true;
        }
        else if(lower_key == "path")
        {
            cookie_entry->second.path = std::string(value_view.data(), value_view.size());
        }
        else if(lower_key == "samesite")
        {
            std::string lower_value;
            
            std::transform(value_view.cbegin(), value_view.cend(), lower_value.end(), [](const char &c) -> char
            {
                return std::tolower(c);
            });

            if(lower_value == "strict")
            {
                cookie_entry->second.samesite_policy = samesite::strict;
            }
            else if(lower_value == "lax")
            {
                cookie_entry->second.samesite_policy = samesite::lax;
            }
            else if(lower_value == "none")
            {
                cookie_entry->second.samesite_policy = samesite::none;
            }
        }
        else if(lower_key == "secure")
        {
            cookie_entry->second.secure = true;
        }
    }
}