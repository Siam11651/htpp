#ifndef RESPONSE_H
#define RESPONSE_H

#include <cstdint>
#include <map>
#include <string>

namespace htpp
{
    class response
    {
    private:
        static const std::map<uint16_t, std::string> s_status_map;

    public:
        uint16_t status_code = 200;
        std::string content_type = "application/octet-stream";
        std::string body;

        const std::string serialize() const;
    };
}

#endif