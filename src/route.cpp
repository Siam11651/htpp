#include <route.hpp>
#include <list>

const std::string htpp::route::s_segment_symbols("-._~!$&'()*+,;=");

bool htpp::route::is_valid_segment(const std::string &segment) const
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
    std::string fragment;
    std::string query;
    std::string path;

    {
        const size_t frag_pos = temp_route.find("#");

        if(frag_pos != std::string::npos)
        {
            fragment = temp_route.substr(frag_pos + 1);
            temp_route = temp_route.substr(0, frag_pos);
        }
    }

    {
        const size_t query_pos = temp_route.find("?");

        if(query_pos != std::string::npos)
        {
            query = temp_route.substr(query_pos + 1);
            temp_route = temp_route.substr(0, query_pos);
        }
    }

    {
        std::list<std::string> segments_list;
        size_t start = 0;
        bool run = true;

        while(run)
        {
            size_t pos = temp_route.find("/", start);

            if(pos == std::string::npos)
            {
                pos = temp_route.size();
                run = false;
            }

            segments_list.push_back(temp_route.substr(start, pos - start));
        }

        m_segements.clear();

        for(std::list<std::string>::const_iterator it = segments_list.cbegin(); it != segments_list.cend(); ++it)
        {
            if(it->size() > 0)
            {
                if(is_valid_segment(*it))
                {
                    m_segements.push_back(*it);
                }
                else
                {
                    m_healthy = false;

                    return;
                }
            }
        }
    }
}