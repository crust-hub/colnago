#include <iostream>
#include "server/m_server.h"
int main(int argc, char **argv)
{
    colnago::server::server.start();
    return EXIT_SUCCESS;
}