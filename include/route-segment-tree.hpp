#ifndef ROUTE_SEGMENT_TREE_H
#define ROUTE_SEGMENT_TREE_H

#include <handler.hpp>

namespace htpp
{
    class route::segment_tree_node
    {
    private:
        handler *m_handler_get = nullptr;
        handler *m_handler_post = nullptr;
        handler *m_handler_put = nullptr;
        handler *m_handler_delete = nullptr;

    public:
        std::map<std::string, segment_tree_node *> children;

        void set_handler_get(const handler &handler_get);
        void set_handler_get(const handler &&handler_get);
        const handler *get_handler_get_ptr() const;
        ~segment_tree_node();
    };
}

#endif