#pragma once

#include "../../lib/Application/Application.h"
#include <string>

QT_FORWARD_DECLARE_CLASS(QWebSocket)

namespace nClient {
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
    /** Triggers when the application is connected to the server*/
    void OnConnected();

    /** Triggers when the application is connected to the server*/
    void OnDisconnected();

    /** Triggers when the application receives a message from the server*/
    void OnMessageReceived(const QString& iMessage);

private:
    ::std::string mHost;
    int mPort;
    ::std::string mName;
    bool mAuto;
    QWebSocket* mSocket;
};

}
}