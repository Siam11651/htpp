#include <handler.hpp>

const bool htpp::handler::has_valid_segments() const
{
    for(const route::segment &segment : m_segments)
    {
        if(!segment.is_valid())
        {
            return false;
        }
    }

    return true;
}

htpp::handler::handler(const std::vector<route::segment> &segments, const std::function<const response(const request &)> &routine)
{
    m_segments = segments;

    if(has_valid_segments())
    {
        m_routine = routine;
    }
    else
    {
        m_valid = false;
    }
}

htpp::handler::handler(const handler &other) : m_segments(other.m_segments), m_routine(other.m_routine)
{
    if(!has_valid_segments())
    {
        m_valid = false;
    }
}

htpp::handler::handler(const handler &&other) : m_segments(std::move(other.m_segments)), m_routine(std::move(other.m_routine))
{
    if(!has_valid_segments())
    {
        m_valid = false;
    }
}

const htpp::response htpp::handler::handle(const request &req) const
{
    return m_routine(req);
}

const std::vector<htpp::route::segment> &htpp::handler::get_segments() const
{
    return m_segments;
}

const bool &htpp::handler::is_valid() const
{
    return m_valid;
}