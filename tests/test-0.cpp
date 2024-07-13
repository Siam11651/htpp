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
        server_builder.docroot = "/mnt/hdd/sources/htpp/docroot/0";
        server_builder.port = 5173;
        server = new htpp::htpp(server_builder);
    }

    server->run();

    return 0;
}