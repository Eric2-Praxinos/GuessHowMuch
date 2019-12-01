#pragma once

#include <QtCore/QObject>

QT_FORWARD_DECLARE_CLASS(QWebSocket)

namespace nServer {
namespace nApplication {

class cSession : public QObject {
Q_OBJECT
public:
    /** Destructor */
    ~cSession();

    /** Constructor */
    cSession(QWebSocket* iSocket);

Q_SIGNALS:
    /** Session closed signal */
    void Closed();

private Q_SLOTS:
    /** Triggers when the session receives a message from the client */
    void OnMessageReceived(const QString& iMessage);

    /** Triggers when a client connects */
    void OnClientDisconnected();

private:
    QWebSocket* mSocket;
};

}
}
