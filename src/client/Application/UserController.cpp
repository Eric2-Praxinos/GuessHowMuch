#include "UserController.h"
#include "Session.h"
#include <QtCore/QDateTime>
#include <iostream>
#include <limits>


namespace nClient {
namespace nApplication {

/** Destructor */
cUserController::~cUserController() {

}

/** Constructor */
cUserController::cUserController(cSession* iSession) :
    cController(iSession)
{
    connect(Session(), SIGNAL(CommandReceived(const ::nShared::nSession::cCommand&)), this, SLOT(OnCommandReceived(const ::nShared::nSession::cCommand&)));
}

/** Triggers when the application receives a message from the server*/
void
cUserController::OnCommandReceived(const ::nShared::nSession::cCommand& iCommand) {
    switch(iCommand.Type()) {
        case ::nShared::nSession::cCommand::kRules: {
        }
        break;

        case ::nShared::nSession::cCommand::kRequireFirstGuess: {
            int guess = 0;
            while(!(std::cin >> guess)) {
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::cout << "Invalid Input. Try Again : ";
            }
            Session()->SendGuess(guess);
        }
        break;

        case ::nShared::nSession::cCommand::kHint: {
            int guess = 0;
            while(!(std::cin >> guess)) {
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::cout << "Invalid Input. Try Again : ";
            }
            Session()->SendGuess(guess);
        }
        break;

        default:
        break;
    }
}



}
}
