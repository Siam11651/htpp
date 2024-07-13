#include <route-segment-tree.hpp>

void htpp::route::segment_tree_node::set_handler_get(const handler &handler_get)
{
    m_handler_get = new handler(handler_get);
}

void htpp::route::segment_tree_node::set_handler_get(const handler &&handler_get)
{
    m_handler_get = new handler(handler_get);
}

htpp::route::segment_tree_node::~segment_tree_node()
{
    delete m_handler_get;
    delete m_handler_post;
    delete m_handler_put;
    delete m_handler_delete;
}