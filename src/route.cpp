#include <route.hpp>

htpp::route::route(const std::string_view &route)
{
    std::string_view temp_route(route);
    std::string_view fragment;
    std::string_view query;
    std::string_view path;

    {
        const size_t frag_pos = temp_route.find("#");

        if(frag_pos != std::string_view::npos)
        {
            fragment = temp_route.substr(frag_pos + 1);
            temp_route.remove_suffix(temp_route.size() - frag_pos);
        }
    }

    {
        const size_t query_pos = temp_route.find("?");

        if(query_pos != std::string_view::npos)
        {
            query = temp_route.substr(query_pos + 1);
            temp_route.remove_suffix(temp_route.size() - query_pos);
        }
    }

    path = temp_route;
}