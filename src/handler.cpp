#include <handler.hpp>

htpp::handler::handler(const std::vector<route::segment> &segments, const std::function<const response(const request &)> &routine)
{
    m_segments = segments;
    m_routine = routine;
}

const htpp::response htpp::handler::handle(const request &req) const
{
    return m_routine(req);
}