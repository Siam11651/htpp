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
        std::vector<route::segment> m_segments;
        std::function<const response(const request &)> m_routine;

    public:
        handler(const std::vector<route::segment> &segments, const std::function<const response(const request &)> &routine);
        const response handle(const request &req) const;
    };
}

#endif