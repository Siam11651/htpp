#include <request.hpp>
#include <vector>
#include <ranges>
#include <algorithm>
#include <sstream>
#include <iostream>

bool htpp::request::is_integer(const std::string &query) const
{
    if(query.size() == 0)
    {
        return false;
    }

    for(const char &c : query)
    {
        if(!('0' <= c && c <= '9'))
        {
            return false;
        }
    }

    return true;
}

const std::string_view htpp::request::trim_string_view(const std::string_view &str) const
{
    size_t left_last_ws = std::string_view::npos;

    for(size_t i = 0; i < str.size(); ++i)
    {
        const char &c = str[i];

        if(c == '\t' || c == '\n' || c == '\v' || c == '\f' || c == '\r' || c == ' ')
        {
            left_last_ws = i;
        }
        else
        {
            break;
        }
    }

    std::string_view to_return = str.substr(left_last_ws + 1);
    size_t right_first_ws = to_return.size();

    for(size_t i = to_return.size() - 1; i != std::string::npos; --i)
    {
        const char &c = to_return[i];

        if(c == '\t' || c == '\n' || c == '\v' || c == '\f' || c == '\r' || c == ' ')
        {
            right_first_ws = i;
        }
        else
        {
            break;
        }
    }

    return to_return.substr(0, right_first_ws);
}

htpp::request::request(const std::string &message)
{
    std::string temp_message(message);
    const size_t request_line_end_pos = temp_message.find("\r\n");

    if(request_line_end_pos == std::string::npos)
    {
        m_valid = false;

        return;
    }
    
    const std::string request_line(temp_message.substr(0, request_line_end_pos));
    std::vector<std::string> request_line_components(3);

    {
        size_t next_component = 0;

        for(const auto &component : std::ranges::views::split(request_line, std::string(" ")))
        {
            if(next_component == 3)
            {
                m_valid = false;

                return;
            }

            request_line_components[next_component] = std::string(component.data(), component.size());
            ++next_component;
        }

        if(next_component < 3)
        {
            m_valid = false;

            return;
        }
    }

    if(request_line_components[0] == "OPTIONS")
    {
        m_method = method::OPTIONS;
    }
    else if(request_line_components[0] == "GET")
    {
        m_method = method::GET;
    }
    else if(request_line_components[0] == "HEAD")
    {
        m_method = method::HEAD;
    }
    else if(request_line_components[0] == "POST")
    {
        m_method = method::POST;
    }
    else if(request_line_components[0] == "PUT")
    {
        m_method = method::PUT;
    }
    else if(request_line_components[0] == "DELETE")
    {
        m_method = method::DELETE;
    }
    else if(request_line_components[0] == "TRACE")
    {
        m_method = method::TRACE;
    }
    else
    {
        m_valid = false;

        return;
    }

    m_route = route(request_line_components[1]);

    {
        const size_t version_slash_pos = request_line_components[2].find("/");

        if(version_slash_pos == std::string::npos)
        {
            m_valid = false;

            return;
        }

        const size_t version_dot_pos = request_line_components[2].find(".");

        if(version_dot_pos == std::string::npos)
        {
            m_valid = false;

            return;
        }

        m_http_major = request_line_components[2].substr(version_slash_pos + 1, version_dot_pos - version_slash_pos - 1);
        
        if(!is_integer(m_http_major))
        {
            m_valid = false;

            return;
        }

        m_http_minor = request_line_components[2].substr(version_dot_pos + 1);

        if(!is_integer(m_http_minor))
        {
            m_valid = false;

            return;
        }
    }

    size_t next_start = request_line_end_pos + 2;
    size_t content_length = 0;

    for(; next_start < temp_message.size(); )
    {
        size_t line_end_pos = temp_message.find("\r\n", next_start);
        bool no_crlf_ending = false;

        if(line_end_pos == std::string::npos)
        {
            line_end_pos = temp_message.size();
            no_crlf_ending = true;
        }

        if(next_start == line_end_pos)
        {
            next_start = line_end_pos + 2;

            break;
        }

        const std::string_view line_view(temp_message.data() + next_start, line_end_pos - next_start);
        const size_t colon_pos = line_view.find(":");

        if(colon_pos == std::string_view::npos)
        {
            m_valid = false;

            return;
        }

        const std::string_view header_name_view(trim_string_view(line_view.substr(0, colon_pos)));
        const std::string_view header_value_view(trim_string_view(line_view.substr(colon_pos + 1)));
        std::string header_name(header_name_view.data(), header_name_view.size());
        const std::string header_value(header_value_view.data(), header_value_view.size());

        std::transform(header_name.cbegin(), header_name.cend(), header_name.begin(), [](const char &c) -> char
        {
            return std::tolower(c);
        });
        m_headers.insert({header_name, header_value});

        if(header_name == "content-length")
        {
            std::stringstream ss;

            ss << header_value;
            ss >> content_length;
        }

        if(no_crlf_ending)
        {
            next_start = line_end_pos;
        }
        else
        {
            next_start = line_end_pos + 2;
        }
    }
}

const htpp::request::method &htpp::request::get_method() const
{
    return m_method;
}

const htpp::route &htpp::request::get_route() const
{
    return m_route;
}

const bool htpp::request::is_valid() const
{
    return m_valid;
}

const std::map<std::string, std::string> &htpp::request::get_headers() const
{
    return m_headers;
}