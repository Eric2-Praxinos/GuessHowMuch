#include <stdio.h>
#include "Application/Application.h"

int main(int argc, char** argv) {
    printf("Hello I'm Server\n");
    ::nServer::nApplication::cApplication* application = new ::nServer::nApplication::cApplication();
    application->Launch(argc, argv);

    return 0;
}
