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
}

void
cSession::OnMessageReceived(const QString& iMessage) {
    printf("Message Received %s\n", iMessage.toStdString().c_str());
}

void
cSession::OnClientDisconnected() {
    emit Closed();
}

void
cSession::SendRules() const {
    //Send first command to inform client with the rules
    QJsonObject rulesJson;
    rulesJson["limit"] = mLimit;
    rulesJson["bounds"] = mBounds.ToJson();
    
    ::nShared::nSession::cCommand command(::nShared::nSession::cCommand::kRules, rulesJson);
    mSocket->sendTextMessage(command.ToString());
}

}
}