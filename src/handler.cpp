#include <handler.hpp>

htpp::handler::handler(const std::vector<route::segment> &segments, const std::function<const response(const request &)> &routine)
{
    m_segments = segments;
    m_routine = routine;
}

htpp::handler::handler(const handler &other) : m_segments(other.m_segments), m_routine(other.m_routine)
{

}

htpp::handler::handler(const handler &&other) : m_segments(std::move(other.m_segments)), m_routine(std::move(other.m_routine))
{
    
}

const htpp::response htpp::handler::handle(const request &req) const
{
    return m_routine(req);
}