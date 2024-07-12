#include <request.hpp>
#include <ranges>
#include <vector>
#include <iostream>

bool htpp::request::is_interger(const std::string &query) const
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

htpp::request::request(const std::string &message)
{
    std::string temp_message(message);
    const size_t request_line_end_pos = temp_message.find("\r\n");

    if(request_line_end_pos == std::string::npos)
    {
        m_healthy = false;

        return;
    }
    
    const std::string request_line(temp_message.substr(0, request_line_end_pos + 1));
    std::vector<std::string> request_line_components(3);

    {
        size_t next_component = 0;
        size_t start = 0;
        bool run = true;

        while(run && next_component < 3)
        {
            size_t pos = request_line.find(" ", start);

            if(pos == std::string::npos)
            {
                run = false;
                pos = request_line.size();
            }

            request_line_components[next_component] = request_line.substr(start, pos - start);
            ++next_component;
            start = pos + 1;
        }

        if(next_component < 3 || start <= request_line.size())
        {
            m_healthy = false;

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
        m_healthy = false;

        return;
    }

    m_route = route(request_line_components[1]);

    {
        const size_t version_slash_pos = request_line_components[2].find("/");

        if(version_slash_pos == std::string::npos)
        {
            m_healthy = false;

            return;
        }

        const size_t version_dot_pos = request_line_components[2].find(".");

        if(version_dot_pos == std::string::npos)
        {
            m_healthy = false;

            return;
        }

        m_http_major = request_line_components[2].substr(version_slash_pos + 1, version_dot_pos - version_slash_pos - 1);
        
        if(!is_interger(m_http_major))
        {
            m_healthy = false;

            return;
        }

        m_http_minor = request_line_components[2].substr(version_dot_pos + 1);

        if(!is_interger(m_http_minor))
        {
            m_healthy = false;

            return;
        }
    }
}