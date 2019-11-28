#include <stdio.h>
#include "Application/Application.h"

int main(int argc, char** argv) {
    printf("Hello I'm Client\n");
    ::nClient::nApplication::cApplication* application = new ::nClient::nApplication::cApplication();
    application->Launch(argc, argv);
    return 0;
}
