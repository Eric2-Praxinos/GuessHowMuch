#include "AIController.h"
#include "Session.h"


namespace nClient {
namespace nApplication {

/** Destructor */
cAIController::~cAIController() {

}

/** Constructor */
cAIController::cAIController(cSession* iSession) :
    cController(iSession)
{
    connect(Session(), SIGNAL(CommandReceived(const ::nShared::nSession::cCommand&)), this, SLOT(OnCommandReceived(const ::nShared::nSession::cCommand&)));
}

/** Triggers when the application receives a message from the server*/
void
cAIController::OnCommandReceived(const ::nShared::nSession::cCommand& iCommand) {
    switch(iCommand.Type()) {
        default:
            //TODO:
            break;
    }
}

}
}
