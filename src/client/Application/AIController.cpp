#include "AIController.h"
#include "Session.h"
#include "../../lib/Base/Error.h"
#include <QtCore/QDateTime>


namespace nClient {
namespace nApplication {

/** Destructor */
cAIController::~cAIController() {

}

/** Constructor */
cAIController::cAIController(cSession* iSession) :
    cController(iSession),
    mRandomGenerator(QDateTime::currentMSecsSinceEpoch()),
    mBounds(),
    mGuess(0)
{
    connect(Session(), SIGNAL(CommandReceived(const ::nShared::nSession::cCommand&)), this, SLOT(OnCommandReceived(const ::nShared::nSession::cCommand&)));
}

/** Triggers when the application receives a message from the server*/
void
cAIController::OnCommandReceived(const ::nShared::nSession::cCommand& iCommand) {
    switch(iCommand.Type()) {
        case ::nShared::nSession::cCommand::kRules: { 
            const QJsonObject& value = iCommand.Value();
            mBounds = ::nMath::cRange(value["bounds"]["min"].toInt(), value["bounds"]["max"].toInt());
        }
        break;

        case ::nShared::nSession::cCommand::kRequireFirstGuess: {
            mGuess = mRandomGenerator.bounded(mBounds.Min(), mBounds.Max());
            printf("%d\n", mGuess);
            Session()->SendGuess(mGuess);
        }
        break;

        case ::nShared::nSession::cCommand::kHint: {
            const QJsonObject& value = iCommand.Value();
            if (value["hint"].toString() == "-") {
                mBounds.Max(mGuess-1);
            } else {
                mBounds.Min(mGuess+1);
            }
            mGuess = mRandomGenerator.bounded(mBounds.Min(), mBounds.Max());
            printf("%d\n", mGuess);
            Session()->SendGuess(mGuess);
        }
        break;

        default: {
        }
        break;
    }
}



}
}
