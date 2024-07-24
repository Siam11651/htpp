#ifndef STRING_VIEW_H
#define STRING_VIEW_H

#include <string_view>

namespace htpp
{
    class string_view : public std::string_view
    {
    public:
        string_view();
        string_view(const std::string_view &other);
        string_view(const char *str, const size_t &len);
        void trim(const std::string_view &target = " \r\n\t\v\f");
    };
}

#endif