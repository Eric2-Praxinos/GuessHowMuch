#include "ParserInterval.h"
#include <stdexcept>

namespace nBase {

cParserInterval::~cParserInterval() {
    delete mParserInteger;
}

cParserInterval::cParserInterval() : 
    cParser<::nMath::cRange>(),
    mParserInteger(new cParserInteger())
{
}

::nMath::cRange
cParserInterval::Parse(const ::std::string& iString, const ::nMath::cRange& iDefaultValue) const {
    int index = iString.find(',');
    if (index == ::std::string::npos || index <= 0 || index >= iString.length() - 1) {
        throw new std::invalid_argument("Value is not a valid range");
    }

    std::string bound1 = iString.substr(0, index);
    std::string bound2 = iString.substr(index+1, iString.length() - index - 1);

    int intBound1 = mParserInteger->Parse(bound1, 0);
    int intBound2 = mParserInteger->Parse(bound2, 0);

    return ::nMath::cRange(intBound1, intBound2);
}

} //namespace nBase