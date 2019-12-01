#include "Application.h"
#include "Session.h"
#include "AIController.h"
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

    ::std::string url = "ws://" + mHost + ":" + ::std::to_string(mPort);

    mSession = new cSession(QString::fromStdString(mName));

    if (mAuto) {
        mController = new cAIController(mSession);
    } else {
    }
    mSession->Open(QUrl(url.c_str()));

    app.exec();
}

}
}