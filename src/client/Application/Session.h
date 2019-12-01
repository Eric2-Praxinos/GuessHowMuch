#pragma once

#include <QtCore/QObject>
#include <QtCore/QJsonObject>
#include <QtWebSockets/QWebSocket>
#include "../../shared/Command.h"

QT_FORWARD_DECLARE_CLASS(QWebSocket)

namespace nClient {
namespace nApplication {

class cSession : public QObject {
Q_OBJECT

public:
    /** Destructor */
    ~cSession();

    /** Constructor */
    cSession(const QString& iName);

public:
    /** Opens the session */
    void Open(const QUrl& iUrl);

Q_SIGNALS:
    /** Session closed signal */
    void Opened();

    /** Session closed signal */
    void Closed();

    /** Session closed signal */
    void CommandReceived(const ::nShared::nSession::cCommand& iCommand);

private Q_SLOTS:
    /** Triggers when a client connects */
    void OnConnected();

    /** Triggers when the session receives a message from the client */
    void OnMessageReceived(const QString& iCommand);

    /** Triggers when a client connects */
    void OnDisconnected();

    /** Triggers when a socket error occurs */
    void OnSocketError(QAbstractSocket::SocketError iError);

public:
    /** Sends client's informations to the server */
    void SendClientInfos() const;

    /** Sends a guess to the server */
    void SendGuess(int iGuess) const;

private:
    /** Prints the user highscores contained in iArray */
    void PrintHighScores(QJsonArray iArray);

private:
    QWebSocket* mSocket;
    QString mName;
};

}
}
