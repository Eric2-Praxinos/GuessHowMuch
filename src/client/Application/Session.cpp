#include "Session.h"
#include <QtWebSockets/QWebSocket>
#include <QtCore/QJsonDocument>

namespace nClient {
namespace nApplication {

/** Destructor */
cSession::~cSession() {
    delete mSocket;
}

/** Constructor */
cSession::cSession() :
    QObject(),
    mSocket(new QWebSocket())
{
}

void
cSession::Open(const QUrl& iUrl) {
    connect(mSocket, SIGNAL(connected()), this, SLOT(OnConnected()));
    connect(mSocket, SIGNAL(disconnected()), this, SLOT(OnDisconnected()));
    connect(mSocket, SIGNAL(textMessageReceived(const QString&)), this, SLOT(OnMessageReceived(const QString&)));
    mSocket->open(iUrl);
}

void
cSession::OnMessageReceived(const QString& iMessage) {
    ::nShared::nSession::cCommand command(iMessage);

    switch(command.Type()) {
        case ::nShared::nSession::cCommand::kRules: {
            const QJsonObject& value = command.Value();
            printf("Welcome to GuessHowMuch !\n");
            printf("Rules are the following :\n");
            printf("- You have to guess the number the Server chose\n");
            printf("- The number is between %d and %d (included)\n", value["bounds"]["min"].toInt(), value["bounds"]["max"].toInt());
            if (value["limit"].toInt() == -1) {
                printf("- You have unlimited tries to find the number\n");
            } else {
                printf("- You have only %d tries to find the number\n", value["limit"].toInt());
            }
        }
        break;

        case ::nShared::nSession::cCommand::kUnknown: {
            //TODO:
        }
        break;
        default:
            emit CommandReceived(command);
            break;
    }

}

void
cSession::OnConnected() {
    emit Opened();
}

void
cSession::OnDisconnected() {
    emit Closed();
}

}
}