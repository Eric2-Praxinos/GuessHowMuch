#pragma once

#include "../../lib/Application/Application.h"
#include "../../lib/Math/Range.h"
#include <string>

QT_FORWARD_DECLARE_CLASS(QWebSocketServer)
QT_FORWARD_DECLARE_CLASS(QWebSocket)

namespace nServer {
namespace nApplication {

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
    /** Triggers when a client connects*/
    void onClientConnected();

    /** Triggers when a client disconnects*/
    void onClientDisconnected();

private:
    int mPort;
    int mLimit;
    ::nMath::cRange mBounds;
    QWebSocketServer* mSocket;
    std::vector<QWebSocket*> mClients;
};

}
}