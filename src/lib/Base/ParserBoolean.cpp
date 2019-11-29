#include "ParserBoolean.h"

namespace nBase {

cParserBoolean::~cParserBoolean() {
}

cParserBoolean::cParserBoolean(bool iDefaultValue) : 
    cParser<bool>(),
    mDefaultValue(iDefaultValue)
{
}

bool
cParserBoolean::Parse(const ::std::string& iString, const bool& iDefaultValue) const {
    if (iString == "true") {
        return true;
    } else if (iString == "false") {
        return false;
    } else {
        return mDefaultValue;
    }
}

} //namespace nBase