#include "Application.h"
#include "../../lib/Application/OptionParser.h"
#include "../../lib/Base/ParserBoolean.h"
#include "../../lib/Base/ParserInteger.h"
#include "../../lib/Base/ParserString.h"
#include "../../lib/Math/Range.h"
#include <QtCore/QCoreApplication>
#include <QtWebSockets/QWebSocket>

namespace nClient {
namespace nApplication {

/** Destructor */
cApplication::~cApplication() {
    delete mSocket;
}

/** Constructor */
cApplication::cApplication() : 
    ::nApplication::cApplication(),
    mHost(),
    mPort(0),
    mName(),
    mAuto(false),
    mSocket(new QWebSocket())
{
}

void
cApplication::Launch(int argc, char** argv) {
    QCoreApplication app(argc, argv);

    ::nApplication::cOptionParser* optionParser = OptionParser();
    optionParser->AddOption("-h", new ::nBase::cParserString(), &mHost, ::std::string("localhost"));
    optionParser->AddOption("--host", new ::nBase::cParserString(), &mHost, ::std::string("localhost"));
    optionParser->AddOption("-p", new ::nBase::cParserInteger(::nMath::cRange(1, 65535)), &mPort, 4242);
    optionParser->AddOption("--port", new ::nBase::cParserInteger(::nMath::cRange(1, 65535)), &mPort, 4242);
    optionParser->AddOption("-n", new ::nBase::cParserString(), &mName, ::std::string(""));
    optionParser->AddOption("--name", new ::nBase::cParserString(), &mName, ::std::string(""));
    optionParser->AddOption("-a", new ::nBase::cParserBoolean(true), &mAuto, false);
    optionParser->AddOption("--auto", new ::nBase::cParserBoolean(true), &mAuto, false);
    optionParser->Parse(argc, argv); 

    printf("mHost = %s\n", mHost.c_str());
    printf("mPort = %d\n", mPort);
    printf("mName = %s\n", mName.c_str());
    printf("mAuto = %s\n", mAuto ? "true" : "false");

    ::std::string url = "ws://" + mHost + ":" + ::std::to_string(mPort);

    connect(mSocket, SIGNAL(connected()), this, SLOT(OnConnected()));
    connect(mSocket, SIGNAL(disconnected()), this, SLOT(OnDisconnected()));
    mSocket->open(QUrl(url.c_str()));

    app.exec();
}

void
cApplication::OnConnected() {
    printf("Socket Connected\n");
    connect(mSocket, SIGNAL(textMessageReceived(const QString&)), this, SLOT(OnMessageReceived(const QString&)));
    mSocket->sendTextMessage("My Name is the Doctor.");
}

void
cApplication::OnDisconnected() {
    printf("Socket Disconnected\n");
}

void
cApplication::OnMessageReceived(const QString& iMessage) {
    printf("Message Received %s\n", iMessage.toStdString().c_str());
}

}
}