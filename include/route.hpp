#ifndef URL_H
#define URL_H

#include <string>
#include <vector>
#include <map>
#include <functional>

namespace htpp
{
    class route
    {
    public:
        class segment
        {
        private:
            static const std::string s_segment_symbols;
            bool m_valid = true;
            bool m_variable = false;
            std::string m_name;

        public:
            segment();
            segment(const std::string &name);
            const bool &is_valid() const;
            const bool &is_variable() const;
            const std::string &get_name() const;
        };

        class segment_tree_node;

    private:
        bool m_valid = true;
        std::vector<route::segment> m_segements;
        std::string m_query;
    
    public:
        route(const std::string &route = "*");
        const std::vector<segment> &get_segments() const;
        std::string get_path() const;
    };
}

#endif