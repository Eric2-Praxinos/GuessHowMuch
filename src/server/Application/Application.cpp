#include "Application.h"
#include "Session.h"
#include "../../lib/Application/OptionParser.h"
#include "../../lib/Base/ParserBoolean.h"
#include "../../lib/Base/ParserInteger.h"
#include "../../lib/Base/ParserInterval.h"
#include "../../lib/Base/ParserString.h"
#include "../../lib/Math/Range.h"
#include <QtCore/QCoreApplication>
#include <QtWebSockets/QWebSocketServer>
#include <QtWebSockets/QWebSocket>

namespace nServer {
namespace nApplication {

/** Destructor */
cApplication::~cApplication() {

}

/** Constructor */
cApplication::cApplication() : 
    ::nApplication::cApplication(),
    mPort(0),
    mLimit(0),
    mBounds(),
    mSocket(new QWebSocketServer("GuessHowMuch Server", QWebSocketServer::NonSecureMode, this)),
    mClients()
{
}

void
cApplication::Launch(int argc, char** argv) {
    QCoreApplication app(argc, argv);

    ::nApplication::cOptionParser* optionParser = OptionParser();
    optionParser->AddOption("-p", new ::nBase::cParserInteger(::nMath::cRange(1, 65535)), &mPort, 4242);
    optionParser->AddOption("--port", new ::nBase::cParserInteger(::nMath::cRange(1, 65535)), &mPort, 4242);
    optionParser->AddOption("-l", new ::nBase::cParserInteger(::nMath::cRange(-1, INT_MAX)), &mLimit, -1);
    optionParser->AddOption("--limit", new ::nBase::cParserInteger(::nMath::cRange(1, INT_MAX)), &mLimit, -1);
    optionParser->AddOption("-b", new ::nBase::cParserInterval(), &mBounds, ::nMath::cRange(1, 100));
    optionParser->AddOption("--bounds", new ::nBase::cParserInterval(), &mBounds, ::nMath::cRange(1, 100));
    optionParser->Parse(argc, argv); 

    printf("mPort = %d\n", mPort);
    printf("mLimit = %d\n", mLimit);
    printf("mBounds = %d, %d\n", mBounds.Min(), mBounds.Max());

    connect(mSocket, SIGNAL(newConnection()), this, SLOT(OnClientConnected()));
    mSocket->listen(QHostAddress::Any, mPort);

    app.exec();
}

void
cApplication::OnClientConnected() {
    printf("New Client Connected\n");
    cSession* session = new cSession(mSocket->nextPendingConnection());
    connect(session, SIGNAL(Closed()), this, SLOT(OnSessionClosed()));
}

void
cApplication::OnSessionClosed() {
    printf("Session Closed\n");
}

}
}