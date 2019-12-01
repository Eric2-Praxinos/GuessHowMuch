#pragma once

#include <QtCore/QString>
#include <QtCore/QJsonObject>
#include <QtCore/QJsonDocument>

namespace nShared {
namespace nSession {

class cCommand {
public:
enum eType {
    kUnknown = 0,
    kError,
    kClientInfos,
    kRules,
    kRequireFirstGuess,
    kGuess,
    kHint,
    kSuccess,
    kFailure
};

public:
    /** Destructor */
    ~cCommand();

    /** Constructor */
    cCommand(const QString& iJsonString);

    /** Constructor */
    cCommand(eType iType, const QJsonObject& iValue);

public:
    /** Returns the command value */
    const QJsonObject& Value() const;

    /** Returns the command type */
    eType Type() const;

    /** Returns a String of the command */
    QString ToString() const;

private:
    eType mType;
    QJsonObject mValue;
    QJsonDocument mDocument;
};

}
}
