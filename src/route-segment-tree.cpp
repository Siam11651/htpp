#include <route-segment-tree.hpp>

void htpp::route::segment_tree_node::set_handler_get(const handler &handler_get)
{
    m_handler_get = new handler(handler_get);
}

void htpp::route::segment_tree_node::set_handler_get(const handler &&handler_get)
{
    m_handler_get = new handler(std::move(handler_get));
}

void htpp::route::segment_tree_node::set_handler_post(const handler &handler_post)
{
    m_handler_post = new handler(handler_post);
}

void htpp::route::segment_tree_node::set_handler_post(const handler &&handler_post)
{
    m_handler_post = new handler(std::move(handler_post));
}

void htpp::route::segment_tree_node::set_handler_put(const handler &handler_put)
{
    m_handler_put = new handler(handler_put);
}

void htpp::route::segment_tree_node::set_handler_put(const handler &&handler_put)
{
    m_handler_put = new handler(std::move(handler_put));
}

void htpp::route::segment_tree_node::set_handler_delete(const handler &handler_delete)
{
    m_handler_delete = new handler(handler_delete);
}

void htpp::route::segment_tree_node::set_handler_delete(const handler &&handler_delete)
{
    m_handler_delete = new handler(std::move(handler_delete));
}

const htpp::handler *htpp::route::segment_tree_node::get_handler_get_ptr() const
{
    return m_handler_get;
}

const htpp::handler *htpp::route::segment_tree_node::get_handler_post_ptr() const
{
    return m_handler_post;
}

const htpp::handler *htpp::route::segment_tree_node::get_handler_put_ptr() const
{
    return m_handler_put;
}

const htpp::handler *htpp::route::segment_tree_node::get_handler_delete_ptr() const
{
    return m_handler_delete;
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