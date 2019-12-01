#pragma once

#include "../../lib/Application/Application.h"
#include "../../lib/Math/Range.h"
#include <string>
#include <QtCore/QRandomGenerator>

QT_FORWARD_DECLARE_CLASS(QWebSocketServer)
QT_FORWARD_DECLARE_CLASS(QWebSocket)

namespace nServer {
namespace nApplication {

class cSession;

class cApplication : public ::nApplication::cApplication {
Q_OBJECT
public:
    /** Destructor */
    ~cApplication();

    /** Constructor */
    cApplication();

public:
    /** Launch the application */
    void Launch(int argc, char** argv);

private Q_SLOTS:
    /** Triggers when a client connects */
    void OnClientConnected();

    /** Triggers when a session closes */
    void OnSessionClosed();

private:
    int mPort;
    int mLimit;
    ::nMath::cRange mBounds;
    QWebSocketServer* mSocket;
    std::vector<cSession*> mSessions;
    QRandomGenerator mRandomGenerator;
};

}
}