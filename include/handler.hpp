#ifndef HANDLER_H
#define HANDLER_H

#include <route.hpp>
#include <request.hpp>
#include <response.hpp>
#include <functional>

namespace htpp
{
    class handler
    {
    private:
        bool m_valid = true;
        std::vector<route::segment> m_segments;
        std::function<const response(const request &)> m_routine;

        const bool has_valid_segments() const;

    public:
        handler(const std::vector<route::segment> &segments, const std::function<const response(const request &)> &routine);
        handler(const handler &other);
        handler(const handler &&other);
        response handle(const request &req) const;
        const std::vector<route::segment> &get_segments() const;
        const bool &is_valid() const;
    };
}

#endif