#include "Application.h"
#include "Session.h"
#include "../../lib/Application/OptionParser.h"
#include "../../lib/Base/ParserBoolean.h"
#include "../../lib/Base/ParserInteger.h"
#include "../../lib/Base/ParserInterval.h"
#include "../../lib/Base/ParserString.h"
#include "../../lib/Base/Error.h"
#include "../../lib/Math/Range.h"
#include <QtCore/QCoreApplication>
#include <QtWebSockets/QWebSocketServer>
#include <QtWebSockets/QWebSocket>

namespace nServer {
namespace nApplication {

/** Destructor */
cApplication::~cApplication() {
    // Sessions deletion
    for (std::vector<cSession*>::iterator it = mSessions.begin(); it != mSessions.end(); it++) {
        delete *it;
    }
}

/** Constructor */
cApplication::cApplication() : 
    ::nApplication::cApplication(),
    mPort(0),
    mLimit(0),
    mBounds(),
    mSocket(nullptr),
    mSessions(),
    mRandomGenerator(QDateTime::currentMSecsSinceEpoch())
{
}

void
cApplication::Launch(int argc, char** argv) {
    QCoreApplication app(argc, argv);

    //Option parsing
    try {
        ::nApplication::cOptionParser* optionParser = OptionParser();
        optionParser->AddOption("-p", new ::nBase::cParserInteger(::nMath::cRange(1, 65535)), &mPort, 4242);
        optionParser->AddOption("--port", new ::nBase::cParserInteger(::nMath::cRange(1, 65535)), &mPort, 4242);
        optionParser->AddOption("-l", new ::nBase::cParserInteger(::nMath::cRange(-1, INT_MAX)), &mLimit, -1);
        optionParser->AddOption("--limit", new ::nBase::cParserInteger(::nMath::cRange(1, INT_MAX)), &mLimit, -1);
        optionParser->AddOption("-b", new ::nBase::cParserInterval(), &mBounds, ::nMath::cRange(1, 100));
        optionParser->AddOption("--bounds", new ::nBase::cParserInterval(), &mBounds, ::nMath::cRange(1, 100));
        optionParser->Parse(argc, argv);
    } catch (cError iError) {
        printf("%s\n", iError.Message().c_str());
        return;
    } 

    if (mLimit == 0) {
        printf("Specified limit cannot be 0");
        return;
    }

    // Create server socket
    mSocket = new QWebSocketServer("GuessHowMuch Server", QWebSocketServer::NonSecureMode, this);
    connect(mSocket, SIGNAL(newConnection()), this, SLOT(OnClientConnected()));
    connect(mSocket, SIGNAL(serverError(QWebSocketProtocol::CloseCode)), this, SLOT(OnServerError(QWebSocketProtocol::CloseCode)));
    printf("Listening...\n");

    //Listen for client connections
    bool success = mSocket->listen(QHostAddress::Any, mPort);
    if (!success) {
        return;
    }

    // app loop
    app.exec();
}

void
cApplication::OnClientConnected() {
    // Create a new session for each new client connection
    cSession* session = new cSession(mSocket->nextPendingConnection(), mLimit, mBounds, mRandomGenerator);
    connect(session, SIGNAL(Closed()), this, SLOT(OnSessionClosed()));
    mSessions.push_back(session);
    session->Open();
}

void
cApplication::OnServerError(QWebSocketProtocol::CloseCode iCloseCode) {
    printf("%s\n", mSocket->errorString().toStdString().c_str());
}

void
cApplication::OnAcceptError(QAbstractSocket::SocketError iSocketError) {
    printf("%s\n", mSocket->errorString().toStdString().c_str());
}

void
cApplication::OnSessionClosed() {
    //Cleanup session when it's closed
    for (std::vector<cSession*>::iterator it = mSessions.begin(); it != mSessions.end(); it++) {
        if (*it == sender()) {
            mSessions.erase(it);
            break;
        }
    }
    delete sender();
}

}
}