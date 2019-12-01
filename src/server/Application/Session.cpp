#include "Session.h"
#include "../../shared/Command.h"
#include <QtWebSockets/QWebSocket>
#include <QtCore/QJsonDocument>

namespace nServer {
namespace nApplication {

/** Destructor */
cSession::~cSession() {

}

/** Constructor */
cSession::cSession(QWebSocket* iSocket, int iLimit, const ::nMath::cRange& iBounds, QRandomGenerator& iRandomGenerator) :
    QObject(),
    mSocket(iSocket),
    mNumber(0),
    mLimit(iLimit),
    mBounds(iBounds),
    mTryCount(0)
{
    mNumber = iRandomGenerator.bounded(iBounds.Min(), iBounds.Max());
}

void
cSession::Open() {
    printf("Open Session with number : %d\n", mNumber);
    connect(mSocket, SIGNAL(disconnected()), this, SLOT(OnClientDisconnected()));
    connect(mSocket, SIGNAL(textMessageReceived(const QString&)), this, SLOT(OnMessageReceived(const QString&)));
    
    SendRules();
    SendRequireFirstGuess();
}

void
cSession::OnMessageReceived(const QString& iMessage) {
    ::nShared::nSession::cCommand command(iMessage);

    switch(command.Type()) {
        case ::nShared::nSession::cCommand::kGuess: {
            int guess = command.Value()["guess"].toInt();
            if (guess == mNumber) {
                SendSuccess();
                return;
            }
            
            if (mLimit > -1) {
                mTryCount++;
                if (mTryCount >= mLimit) {
                    SendFailure();
                    return;
                }
            }
            
            if (guess > mNumber) {
                SendHint("-");
            } else {
                SendHint("+");
            }
        }
        break;

        case ::nShared::nSession::cCommand::kUnknown: {
            //TODO:
        }
        break;
        default:
            break;
    }
}

void
cSession::OnClientDisconnected() {
    emit Closed();
}

void
cSession::SendRules() const {
    //Send first command to inform client with the rules
    QJsonObject valueJson;
    valueJson["limit"] = mLimit;
    valueJson["bounds"] = mBounds.ToJson();
    
    ::nShared::nSession::cCommand command(::nShared::nSession::cCommand::kRules, valueJson);
    mSocket->sendTextMessage(command.ToString());
}

void
cSession::SendRequireFirstGuess() const {
    QJsonObject valueJson;
    ::nShared::nSession::cCommand command(::nShared::nSession::cCommand::kRequireFirstGuess, valueJson);
    mSocket->sendTextMessage(command.ToString());
}

void
cSession::SendHint(const QString& iHint) const {
    QJsonObject valueJson;
    valueJson["hint"] = iHint;
    if (mLimit >= -1) {
        valueJson["triesLeft"] = mLimit - mTryCount;
    }
    ::nShared::nSession::cCommand command(::nShared::nSession::cCommand::kHint, valueJson);
    mSocket->sendTextMessage(command.ToString());
}

void
cSession::SendSuccess() const {
    QJsonObject valueJson;
    valueJson["number"] = mNumber;
    ::nShared::nSession::cCommand command(::nShared::nSession::cCommand::kSuccess, valueJson);
    mSocket->sendTextMessage(command.ToString());
}

void
cSession::SendFailure() const {
    QJsonObject valueJson;
    valueJson["number"] = mNumber;
    ::nShared::nSession::cCommand command(::nShared::nSession::cCommand::kFailure, valueJson);
    mSocket->sendTextMessage(command.ToString());
}

}
}