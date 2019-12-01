#include "Session.h"
#include <QtWebSockets/QWebSocket>

namespace nServer {
namespace nApplication {

/** Destructor */
cSession::~cSession() {

}

/** Constructor */
cSession::cSession(QWebSocket* iSocket) :
    QObject(),
    mSocket(iSocket)
{
    connect(mSocket, SIGNAL(disconnected()), this, SLOT(OnClientDisconnected()));
    connect(mSocket, SIGNAL(textMessageReceived(const QString&)), this, SLOT(OnMessageReceived(const QString&)));
}

void
cSession::OnMessageReceived(const QString& iMessage) {
    printf("Message Received %s\n", iMessage.toStdString().c_str());
    mSocket->sendTextMessage("Doctor Who ?");
}

void
cSession::OnClientDisconnected() {
    emit Closed();
}

}
}