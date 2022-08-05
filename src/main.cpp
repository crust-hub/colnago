#include <iostream>
#include "server/m_server.h"
#include "router/entity/response.h"
int main(int argc, char **argv)
{
    colnago::server::server.start();
    return EXIT_SUCCESS;
}