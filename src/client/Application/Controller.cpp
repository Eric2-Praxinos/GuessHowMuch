#include "Controller.h"
#include "Session.h"

namespace nClient {
namespace nApplication {

/** Destructor */
cController::~cController() {

}

/** Constructor */
cController::cController(cSession* iSession) :
    QObject(),
    mSession(iSession)
{

}

/** Triggers when the application receives a message from the server*/
cSession*
cController::Session() {
    return mSession;
}

}
}
