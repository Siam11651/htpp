#include <route-segment-tree.hpp>

void htpp::route::segment_tree_node::set_handler_get(const handler &handler_get)
{
    m_handler_get = new handler(handler_get);
}

void htpp::route::segment_tree_node::set_handler_get(const handler &&handler_get)
{
    m_handler_get = new handler(handler_get);
}

const htpp::handler *htpp::route::segment_tree_node::get_handler_get_ptr() const
{
    return m_handler_get;
}

htpp::route::segment_tree_node::~segment_tree_node()
{
    delete m_handler_get;
    delete m_handler_post;
    delete m_handler_put;
    delete m_handler_delete;

    for(std::map<std::string, segment_tree_node *>::iterator it = children.begin(); it != children.end(); ++it)
    {
        delete it->second;
    }
}