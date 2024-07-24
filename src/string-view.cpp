#include <string-view.hpp>

htpp::string_view::string_view() : std::string_view()
{

}

htpp::string_view::string_view(const char *str, const size_t &len) : std::string_view(str, len)
{

}

htpp::string_view::string_view(const std::string_view &other) : std::string_view(other)
{
    
}

void htpp::string_view::trim(const std::string_view &target)
{
    remove_prefix(find_first_not_of(target));
    remove_suffix(size() - find_first_not_of(target));
}