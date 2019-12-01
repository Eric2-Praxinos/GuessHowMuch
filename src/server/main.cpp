#include <stdio.h>
#include "Application/Application.h"

int main(int argc, char** argv) {
    ::nServer::nApplication::cApplication* application = new ::nServer::nApplication::cApplication();
    application->Launch(argc, argv);
    return 0;
}
