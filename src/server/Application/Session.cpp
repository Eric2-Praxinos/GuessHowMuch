#include "Session.h"
#include "../../shared/Command.h"
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
    mNumber = iRandomGenerator.bounded(iBounds.Min(), iBounds.Max());
}

void
cSession::Open() {
    mStartDateTime = QDateTime::currentDateTime();
    printf("Open Session with number : %d\n", mNumber);
    connect(mSocket, SIGNAL(disconnected()), this, SLOT(OnClientDisconnected()));
    connect(mSocket, SIGNAL(textMessageReceived(const QString&)), this, SLOT(OnMessageReceived(const QString&)));
    
    SendRules();
    SendRequireFirstGuess();
}

void
cSession::OnMessageReceived(const QString& iMessage) {
    ::nShared::nSession::cCommand command(iMessage);

    switch(command.Type()) {
        case ::nShared::nSession::cCommand::kClientInfos: {
            mClientName = command.Value()["clientName"].toString();
        }
        break;

        case ::nShared::nSession::cCommand::kGuess: {
            int guess = command.Value()["guess"].toInt();
            if (guess == mNumber) {
                mStatus = ::nShared::nSession::kSuccess;
                SaveSession();
                SendSuccess();
                mSocket->close();
                return;
            }
            
            mTryCount++;
            
            if (mLimit > -1) {
                if (mTryCount >= mLimit) {
                    mStatus = ::nShared::nSession::kFailure;
                    SaveSession();
                    SendFailure();
                    mSocket->close();
                    return;
                }
            }
            
            if (guess > mNumber) {
                SendHint("-");
            } else {
                SendHint("+");
            }
        }
        break;

        case ::nShared::nSession::cCommand::kUnknown: {
            //TODO:
        }
        break;
        default:
            break;
    }
}

void
cSession::OnClientDisconnected() {
    emit Closed();
}

void
cSession::SendRules() const {
    //Send first command to inform client with the rules
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

bool compareSession(const QJsonObject& iSession1, const QJsonObject& iSession2) {
    return iSession1["tryCount"].toInt() < iSession2["tryCount"].toInt();
}

QJsonArray
cSession::GetUserBestSessions(int iCount) const {
    if (mClientName.size() == 0) {
        return QJsonArray();
    }

    QFile file("sessions.json");
    if (!file.open(QIODevice::ReadOnly)) {
        //TODO:
    }

    QByteArray data = file.readAll();
    file.close();
    QJsonDocument document = QJsonDocument::fromJson(data);
    QJsonArray array = document.array();
    QJsonArray bestScoresArray;

    ::std::vector<QJsonObject> sessions;
    for(QJsonArray::const_iterator it = array.begin(); it != array.end(); it++) {
        QJsonObject session = (*it).toObject();
        if (session["clientName"].toString() == mClientName) {
            sessions.push_back(session);
        }
    }

    std::sort(sessions.begin(), sessions.end(), compareSession);
    if (sessions.size() > iCount ) {
        sessions.resize(iCount);
    }

    for(std::vector<QJsonObject>::iterator it = sessions.begin(); it != sessions.end(); it++) {
        QJsonObject session = (*it);
        bestScoresArray.append(session);
    }
    return bestScoresArray;
}

void
cSession::SaveSession() const {
    if (mClientName.size() == 0) {
        return;
    }
    
    QFile file("sessions.json");
    if (!file.open(QIODevice::ReadWrite)) {
        //TODO:
    }

    QByteArray data = file.readAll();
    QJsonDocument document = QJsonDocument::fromJson(data);

    QJsonObject object;
    object["clientName"] = mClientName;
    object["startDate"] = mStartDateTime.toString("dd/MM/yyyy HH:mm:ss");
    object["endDate"] = QDateTime::currentDateTime().toString("dd/MM/yyyy HH:mm:ss");
    object["tryCount"] = mTryCount;
    object["status"] = mStatus;

    QJsonArray array = document.array();
    array.append(object);

    document.setArray(array);
    file.resize(0); //clears the file;
    file.write(document.toJson(QJsonDocument::Compact));

    file.close();
}

}
}