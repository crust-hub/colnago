#include <iostream>
#include "server/server.h"
#include "entity/response.h"

int main(int argc, char **argv)
{
    colnago::server::server.start();
    return EXIT_SUCCESS;
}