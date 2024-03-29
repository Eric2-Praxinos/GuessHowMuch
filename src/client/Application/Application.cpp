#include "Application.h"
#include "Session.h"
#include "AIController.h"
#include "UserController.h"
#include "../../lib/Application/OptionParser.h"
#include "../../lib/Base/ParserBoolean.h"
#include "../../lib/Base/ParserInteger.h"
#include "../../lib/Base/ParserString.h"
#include "../../lib/Base/Error.h"
#include "../../lib/Math/Range.h"
#include <QtCore/QCoreApplication>
#include <QtWebSockets/QWebSocket>

namespace nClient {
namespace nApplication {

/** Destructor */
cApplication::~cApplication() {
}

/** Constructor */
cApplication::cApplication() : 
    ::nApplication::cApplication(),
    mHost(),
    mPort(0),
    mName(),
    mAuto(false),
    mSession(nullptr),
    mController(nullptr)
{
}

void
cApplication::Launch(int argc, char** argv) {
    QCoreApplication app(argc, argv);

    //Option Parsing
    try {
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
    } catch (cError iError) {
        printf("%s\n", iError.Message().c_str());
        return;
    }

    // Force Anonymous if Auto mode
    if (mAuto) {
        mName = "";
    }

    // Build socket url
    ::std::string url = "ws://" + mHost + ":" + ::std::to_string(mPort);

    //Create Session
    mSession = new cSession(QString::fromStdString(mName));
    connect(mSession, SIGNAL(Closed()), this, SLOT(OnSessionClosed()));
    
    //Create Controller based on auto mode state
    if (mAuto) {
        mController = new cAIController(mSession);
    } else {
        mController = new cUserController(mSession);
    }

    //Open session
    mSession->Open(QUrl(url.c_str()));

    //app loop
    app.exec();
}

void
cApplication::OnSessionClosed() const {
    delete mController;
    delete mSession;
    exit(0);
}

}
}