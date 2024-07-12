#ifndef URL_H
#define URL_H

#include <string>
#include <vector>

namespace htpp
{
    class route
    {
    private:
        static const std::string_view s_segment_symbols;
        bool m_healthy = true;
        std::vector<std::string_view> m_segements;

        bool is_valid_segment(const std::string_view &segment) const;
    
    public:
        route(const std::string_view &route = "*");
    };
}

#endif