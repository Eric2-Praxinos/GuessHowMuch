#include "Session.h"
#include "../../shared/Session/Command.h"
#include "../../lib/Base/Error.h"
#include <QtWebSockets/QWebSocket>
#include <QtCore/QJsonDocument>
#include <QtCore/QJsonArray>
#include <QtCore/QJsonObject>
#include <QtCore/QFile>

namespace nServer {
namespace nApplication {

/** Destructor */
cSession::~cSession() {

}

/** Constructor */
cSession::cSession(QWebSocket* iSocket, int iLimit, const ::nMath::cRange& iBounds, QRandomGenerator& iRandomGenerator) :
    QObject(),
    mSocket(iSocket),
    mNumber(0),
    mLimit(iLimit),
    mBounds(iBounds),
    mTryCount(0),
    mClientName(),
    mStartDateTime(),
    mStatus(::nShared::nSession::kInProgress)
{
    //Choose a number to guess for this session
    mNumber = iRandomGenerator.bounded(iBounds.Min(), iBounds.Max());
}

void
cSession::Open() {
    //Init startDate
    mStartDateTime = QDateTime::currentDateTime();
    
    printf("Open Session with number : %d\n", mNumber);

    // create connections
    connect(mSocket, SIGNAL(disconnected()), this, SLOT(OnClientDisconnected()));
    connect(mSocket, SIGNAL(textMessageReceived(const QString&)), this, SLOT(OnMessageReceived(const QString&)));
}

void
cSession::OnMessageReceived(const QString& iMessage) {
    ::nShared::nSession::cCommand command(iMessage);

    switch(command.Type()) {
        case ::nShared::nSession::cCommand::kClientInfos: {
            mClientName = command.Value()["clientName"].toString();

            //Client is now identified, send rules and require first guess from the client
            SendRules();
            SendRequireFirstGuess();
        }
        break;

        case ::nShared::nSession::cCommand::kGuess: {
            int guess = command.Value()["guess"].toInt();
            //If client guess is correct
            if (guess == mNumber) {
                //Save Session and send success message to the client
                mStatus = ::nShared::nSession::kSuccess;
                SaveSession();
                SendSuccess();
                mSocket->close();
                return;
            }

            //If client guess is not correct, increment tryCount
            mTryCount++;
            
            //if we reached the try limit
            if (mLimit > -1 && mTryCount >= mLimit) {
                //Save Session and send failure message to the client
                mStatus = ::nShared::nSession::kFailure;
                SaveSession();
                SendFailure();
                mSocket->close();
                return;
            }
            
            //Send hint to the client depending on his guess
            if (guess > mNumber) {
                SendHint("-");
            } else {
                SendHint("+");
            }
        }
        break;

        default: {
            //Manage unknown commands
            SendError("Unknown Command");
            mSocket->close();
        }
        break;
    }
}

void
cSession::OnClientDisconnected() {
    //Manage client disconnection to avoid having a dead connection in memory
    emit Closed();
}

void
cSession::SendRules() const {
    QJsonObject valueJson;
    valueJson["limit"] = mLimit;
    valueJson["bounds"] = mBounds.ToJson();
    
    ::nShared::nSession::cCommand command(::nShared::nSession::cCommand::kRules, valueJson);
    mSocket->sendTextMessage(command.ToString());
}

void
cSession::SendRequireFirstGuess() const {
    QJsonObject valueJson;
    ::nShared::nSession::cCommand command(::nShared::nSession::cCommand::kRequireFirstGuess, valueJson);
    mSocket->sendTextMessage(command.ToString());
}

void
cSession::SendHint(const QString& iHint) const {
    QJsonObject valueJson;
    valueJson["hint"] = iHint;
    if (mLimit > -1) {
        valueJson["triesLeft"] = mLimit - mTryCount;
    }
    ::nShared::nSession::cCommand command(::nShared::nSession::cCommand::kHint, valueJson);
    mSocket->sendTextMessage(command.ToString());
}

void
cSession::SendSuccess() const {
    QJsonObject valueJson;
    valueJson["number"] = mNumber;
    if (mClientName.size() != 0) {
        valueJson["bestSessions"] = GetUserBestSessions(5);
    }
    ::nShared::nSession::cCommand command(::nShared::nSession::cCommand::kSuccess, valueJson);
    mSocket->sendTextMessage(command.ToString());
}

void
cSession::SendFailure() const {
    QJsonObject valueJson;
    valueJson["number"] = mNumber;
    if (mClientName.size() != 0) {
        valueJson["bestSessions"] = GetUserBestSessions(5);
    }
    ::nShared::nSession::cCommand command(::nShared::nSession::cCommand::kFailure, valueJson);
    mSocket->sendTextMessage(command.ToString());
}

void
cSession::SendError(const QString& iError) const {
    QJsonObject valueJson;
    valueJson["error"] = iError;
    ::nShared::nSession::cCommand command(::nShared::nSession::cCommand::kError, valueJson);
    mSocket->sendTextMessage(command.ToString());
}

// Session Comparison function for the highscore table generation
bool compareSession(const QJsonObject& iSession1, const QJsonObject& iSession2) {
    if (iSession1["status"].toInt() != iSession2["status"].toInt()) {
        if (iSession1["status"].toInt() == ::nShared::nSession::kSuccess) {
            return true;
        }
        if (iSession2["status"].toInt() == ::nShared::nSession::kSuccess) {
            return false;
        }
        if (iSession1["status"].toInt() == ::nShared::nSession::kFailure) {
            return true;
        }
        if (iSession2["status"].toInt() == ::nShared::nSession::kFailure) {
            return false;
        }
        return false;
    }
    return iSession1["tryCount"].toInt() < iSession2["tryCount"].toInt();
}

QJsonArray
cSession::GetUserBestSessions(int iCount) const {
    //No Highscore for anonymous
    if (mClientName.size() == 0) {
        return QJsonArray();
    }

    // Open File
    QFile file("sessions.json");
    if (!file.open(QIODevice::ReadOnly)) {
        throw cError("filesystem_error", "Failed to Read User's best Sessions");
    }

    //Read data
    QByteArray data = file.readAll();
    file.close();
    QJsonDocument document = QJsonDocument::fromJson(data);
    QJsonArray array = document.array();
    QJsonArray bestScoresArray;

    //get clients sessions only
    ::std::vector<QJsonObject> sessions;
    for(QJsonArray::const_iterator it = array.begin(); it != array.end(); it++) {
        QJsonObject session = (*it).toObject();
        if (session["clientName"].toString() == mClientName) {
            sessions.push_back(session);
        }
    }

    //sort client sessions
    std::sort(sessions.begin(), sessions.end(), compareSession);

    //keep only iCount client sessions
    if (sessions.size() > iCount ) {
        sessions.resize(iCount);
    }

    //return a json array
    for(std::vector<QJsonObject>::iterator it = sessions.begin(); it != sessions.end(); it++) {
        QJsonObject session = (*it);
        bestScoresArray.append(session);
    }
    return bestScoresArray;
}

void
cSession::SaveSession() const {
    //No session saved for Anonymous
    if (mClientName.size() == 0) {
        return;
    }
    
    //Open file
    QFile file("sessions.json");
    if (!file.open(QIODevice::ReadWrite)) {
        throw cError("filesystem_error", "Failed to Save Session");
    }

    //Read data
    QByteArray data = file.readAll();
    QJsonDocument document = QJsonDocument::fromJson(data);

    //Create session json
    QJsonObject object;
    object["clientName"] = mClientName;
    object["startDate"] = mStartDateTime.toString("dd/MM/yyyy HH:mm:ss");
    object["endDate"] = QDateTime::currentDateTime().toString("dd/MM/yyyy HH:mm:ss");
    object["tryCount"] = mTryCount;
    object["status"] = mStatus;

    //Append session to existing json array
    QJsonArray array = document.array();
    array.append(object);

    document.setArray(array);
    //clear the file
    file.resize(0);
    //write data in the file 
    file.write(document.toJson(QJsonDocument::Compact));

    file.close();
}

}
}