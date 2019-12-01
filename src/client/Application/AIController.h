#pragma once

#include "Controller.h"
#include "../../shared/Session/Command.h"
#include "../../lib/Math/Range.h"
#include <QtCore/QRandomGenerator>

namespace nClient {
namespace nApplication {

class cSession;

class cAIController : public cController {
Q_OBJECT
public:
    /** Destructor */
    ~cAIController();

    /** Constructor */
    cAIController(cSession* iSession);

public:

private Q_SLOTS:
    /** Triggers when the application receives a command from the server*/
    void OnCommandReceived(const ::nShared::nSession::cCommand& iCommand);

private:
    QRandomGenerator mRandomGenerator;
    ::nMath::cRange mBounds;
    int mGuess;
};

}
}
