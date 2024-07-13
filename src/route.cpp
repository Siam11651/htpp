#include <route.hpp>
#include <list>
#include <ranges>
#include <iostream>

htpp::route::segment::segment()
{
    m_variable = true;
}

htpp::route::segment::segment(const std::string &name)
{
    m_name = name;

    if(name.empty())
    {
        m_variable = true;
    }
}

const bool &htpp::route::segment::is_valid() const
{
    return m_valid;
}

const bool &htpp::route::segment::is_variable() const
{
    return m_variable;
}

const std::string &htpp::route::segment::get_name() const
{
    return m_name;
}

const std::string htpp::route::s_segment_symbols("-._~!$&'()*+,;=");

bool htpp::route::is_valid_segment(const std::string_view &segment) const
{
    const size_t &size = segment.size();

    for(size_t i = 0; i < size; ++i)
    {
        const char &c = segment[i];

        if(c == '%')
        {
            if(i + 2 < size)
            {
                for(size_t j = i + 1; j <= i + 2; ++j)
                {
                    const char &c1 = segment[i + j];

                    if(!('0' <= c1 && c1 <= '9') && !('A' <= c1 && c1 <= 'F') && !('a' <= c1 && c1 <= 'f'))
                    {
                        return false;
                    }
                }                
            }
            else
            {
                return false;
            }
        }
        else if(!('0' <= c && c <= '9') && !('A' <= c && c <= 'Z') && !('a' <= c && c <= 'z') && s_segment_symbols.find(c) == std::string::npos)
        {
            return false;
        }
    }

    return true;
}

htpp::route::route(const std::string &route)
{
    std::string temp_route(route);
    std::string path;

    {
        const size_t frag_pos = temp_route.find("#");

        if(frag_pos != std::string::npos)
        {
            temp_route = temp_route.substr(0, frag_pos);
        }
    }

    {
        const size_t query_pos = temp_route.find("?");

        if(query_pos != std::string::npos)
        {
            m_query = temp_route.substr(query_pos + 1);
            temp_route = temp_route.substr(0, query_pos);
        }
    }

    m_segements.clear();

    for(const auto &segment : std::views::split(temp_route, std::string("/")))
    {
        const std::string_view segment_view(segment.data(), segment.size());

        if(!segment_view.empty())
        {
            if(is_valid_segment(segment_view))
            {
                m_segements.push_back(route::segment(std::string(segment_view.data(), segment_view.size())));
            }
            else
            {
                m_healthy = false;

                return;
            }
        }
    }
}

const std::string htpp::route::get_path() const
{
    if(m_segements.empty())
    {
        return "/";
    }
    else
    {
        std::string to_return;

        for(const segment &segment : m_segements)
        {
            to_return += "/" + segment.get_name();
        }

        return to_return;
    }
}