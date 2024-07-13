#include <htpp.hpp>

int main()
{
    htpp::htpp_builder server_builder;
    server_builder.docroot = "/mnt/hdd/sources/htpp/docroot";
    server_builder.port = 5173;
    htpp::htpp server(server_builder);

    server.run();

    return 0;
}