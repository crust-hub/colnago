#include <iostream>
#include "server/m_server.h"
int main(int argc, char **argv)
{
    courscpp::server::m_server server;
    server.start();
    return EXIT_SUCCESS;
}