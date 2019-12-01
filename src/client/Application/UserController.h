#pragma once

#include "Controller.h"
#include "../../shared/Command.h"
#include "../../lib/Math/Range.h"
#include <QtCore/QRandomGenerator>

namespace nClient {
namespace nApplication {

class cSession;

class cUserController : public cController {
Q_OBJECT
public:
    /** Destructor */
    ~cUserController();

    /** Constructor */
    cUserController(cSession* iSession);

public:

private Q_SLOTS:
    /** Triggers when the application receives a command from the server*/
    void OnCommandReceived(const ::nShared::nSession::cCommand& iCommand);

private:
};

}
}
