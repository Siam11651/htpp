#include <signal.h>
#include <htpp.hpp>
#include <iostream>

htpp::htpp *server;

void int_handler(int)
{
    delete server;

    exit(0);
}

int main()
{
    signal(SIGINT, int_handler);

    {
        htpp::htpp_builder server_builder;
        server_builder.docroot = "/mnt/hdd/sources/htpp/docroot/1";
        server_builder.port = 5173;
        server = new htpp::htpp(server_builder);
    }

    server->register_request_handler(std::move(htpp::handler(
    {
        htpp::route::segment("abc"),
        htpp::route::segment("def"),
        htpp::route::segment(),
        htpp::route::segment("ghi")
    }, [](const htpp::request &req) -> htpp::response
    {
        const std::map<std::string, std::string> &headers = req.get_headers();

        for(std::map<std::string, std::string>::const_iterator it0 = headers.begin(); it0 != headers.end(); ++it0)
        {
            std::cout << it0->first << " " << it0->second << std::endl;
        }

        htpp::response http_response;
        http_response.body = "hehe";

        return http_response;
    })), htpp::request::method::POST);

    server->run();

    return 0;
}