#pragma once

#include <QtCore/QObject>
#include <QtCore/QRandomGenerator>
#include "../../lib/Math/Range.h"

QT_FORWARD_DECLARE_CLASS(QWebSocket)

namespace nServer {
namespace nApplication {

class cSession : public QObject {
Q_OBJECT
public:
    /** Destructor */
    ~cSession();

    /** Constructor */
    cSession(QWebSocket* iSocket, int iLimit, const ::nMath::cRange& iBounds, QRandomGenerator& iRandomGenerator);

public:
    /** Opens the session */
    void Open();

Q_SIGNALS:
    /** Session closed signal */
    void Closed();

private Q_SLOTS:
    /** Triggers when the session receives a message from the client */
    void OnMessageReceived(const QString& iMessage);

    /** Triggers when a client connects */
    void OnClientDisconnected();

private:
    /** Sends the rules to the client */
    void SendRules() const;

private:
    QWebSocket* mSocket;
    int mNumber;
    int mLimit;
    int mTryCount;
    ::nMath::cRange mBounds;
};

}
}
