#include "Command.h"
#include <QtCore/QJsonDocument>
#include <string>
#include <iostream>

namespace nShared {
namespace nSession {

/** Destructor */
cCommand::~cCommand() {

}

/** Constructor */
cCommand::cCommand(const QString& iJsonString) :
    mType(eType::kUnknown),
    mValue(),
    mDocument()
{
    mDocument = QJsonDocument::fromJson(iJsonString.toUtf8());
    QJsonObject jsonObject = mDocument.object();
    mType = eType(jsonObject["command"].toInt()); 
    mValue = jsonObject["value"].toObject();
}

/** Constructor */
cCommand::cCommand(eType iType, const QJsonObject& iValue) :
    mType(iType),
    mValue(iValue),
    mDocument()
{
    QJsonObject json;
    json["command"] = iType;
    json["value"] = iValue;
    mDocument = QJsonDocument(json);
}

const QJsonObject&
cCommand::Value() const {
    return mValue;
}

cCommand::eType
cCommand::Type() const {
    return mType;
}

QString
cCommand::ToString() const {
    return mDocument.toJson(QJsonDocument::Compact);
}

}
}