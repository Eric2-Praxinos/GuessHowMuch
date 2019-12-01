#include "Session.h"
#include <QtWebSockets/QWebSocket>
#include <QtCore/QJsonDocument>
#include <QtCore/QJsonArray>
#include <QtCore/QJsonObject>
#include <QtCore/QDateTime>
#include "../../shared/Status.h"

namespace nClient {
namespace nApplication {

/** Destructor */
cSession::~cSession() {
    delete mSocket;
}

/** Constructor */
cSession::cSession(const QString& iName) :
    QObject(),
    mSocket(new QWebSocket()),
    mName(iName)
{
}

void
cSession::Open(const QUrl& iUrl) {
    connect(mSocket, SIGNAL(connected()), this, SLOT(OnConnected()));
    connect(mSocket, SIGNAL(disconnected()), this, SLOT(OnDisconnected()));
    connect(mSocket, SIGNAL(textMessageReceived(const QString&)), this, SLOT(OnMessageReceived(const QString&)));
    mSocket->open(iUrl);
}

void
cSession::OnMessageReceived(const QString& iMessage) {
    ::nShared::nSession::cCommand command(iMessage);

    switch(command.Type()) {
        case ::nShared::nSession::cCommand::kRules: {
            const QJsonObject& value = command.Value();
            printf("Hello %s ! Welcome to GuessHowMuch !\n", mName.size() > 0 ? mName.toStdString().c_str() : "Anonymous");
            printf("Rules are the following :\n");
            printf("- You have to guess the number the Server chose\n");
            printf("- The number is between %d and %d (included)\n", value["bounds"]["min"].toInt(), value["bounds"]["max"].toInt());
            if (value["limit"].toInt() == -1) {
                printf("- You have unlimited tries to find the number\n");
            } else {
                printf("- You have only %d tries to find the number\n", value["limit"].toInt());
            }
        }
        break;

        case ::nShared::nSession::cCommand::kRequireFirstGuess: {
            printf("\nMake your first guess : ");
            fflush(stdout);
        }
        break;

        case ::nShared::nSession::cCommand::kHint: {
            const QJsonObject& value = command.Value();

            printf("\nWrong guess ... :(\n");
            if (value["hint"].toString() == "-") {
                printf("The number is lower than your guess.\n");
            } else {
                printf("The number is higher than your guess.\n");
            }

            if (value.contains("triesLeft")) {
                printf("You have %d tries left.\n", value["triesLeft"].toInt());
            }

            printf("Make your next guess : ");
            fflush(stdout);
        }
        break;

        case ::nShared::nSession::cCommand::kFailure: {
            const QJsonObject& value = command.Value();
            printf("\nGAME OVER...\n");
            printf("The number to guess was : %d\n", value["number"].toInt());
            printf("\nYour highscores :\n");
            if (value.contains("bestSessions")) {
                PrintHighScores(value["bestSessions"].toArray());
            }
        }
        break;

        case ::nShared::nSession::cCommand::kSuccess: {
            const QJsonObject& value = command.Value();
            printf("\n\\o/ CONGRATULATIONS ! \\o/\n");
            printf("The number to guess was : %d\n", value["number"].toInt());
            if (value.contains("bestSessions")) {
                PrintHighScores(value["bestSessions"].toArray());
            }
        }
        break;

        case ::nShared::nSession::cCommand::kUnknown: {
            //TODO:
            return;
        }
        break;
    }
    emit CommandReceived(command);
}

void
cSession::OnConnected() {
    SendClientInfos();
    emit Opened();
}

void
cSession::OnDisconnected() {
    emit Closed();
}

void
cSession::SendGuess(int iGuess) const {
    QJsonObject valueJson;
    valueJson["guess"] = iGuess;
    ::nShared::nSession::cCommand command(::nShared::nSession::cCommand::kGuess, valueJson);
    mSocket->sendTextMessage(command.ToString());
}

void
cSession::SendClientInfos() const {
    QJsonObject valueJson;
    valueJson["clientName"] = mName;
    ::nShared::nSession::cCommand command(::nShared::nSession::cCommand::kClientInfos, valueJson);
    mSocket->sendTextMessage(command.ToString());
}

void
cSession::PrintHighScores(QJsonArray iArray) {
    printf("\nYour highscores :\n");

    printf("|%25s|%20s|%20s|%15s|\n", "Score (Low is best)", "Start Date", "End Date", "Status"); 
    for(QJsonArray::const_iterator it = iArray.begin(); it != iArray.end(); it++) {
        QJsonObject session = (*it).toObject();
        QString startDate = session["startDate"].toString();
        QString endDate = session["endDate"].toString();
        printf("|%25d|%20s|%20s|%15s|\n", session["tryCount"].toInt(), startDate.toStdString().c_str(), endDate.toStdString().c_str(), StatusName(::nShared::nSession::eStatus(session["status"].toInt())).toStdString().c_str());
    }
}

}
}