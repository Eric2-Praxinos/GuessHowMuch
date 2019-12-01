#include "ParserInterval.h"
#include <stdexcept>
#include "Error.h"

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
    // Parce an interval with format [int1,int2]
    int index = iString.find(',');
    if (index == ::std::string::npos || index <= 0 || index >= iString.length() - 1) {
        throw cError("invalid_argument", "Value is not a valid integer range");
    }

    std::string bound1 = iString.substr(0, index);
    std::string bound2 = iString.substr(index+1, iString.length() - index - 1);

    try {
        int intBound1 = mParserInteger->Parse(bound1, 0);
        int intBound2 = mParserInteger->Parse(bound2, 0);

        return ::nMath::cRange(intBound1, intBound2);
    } catch (cError iError) {
        throw cError("invalid_argument", "Range contains invalid values : " + iError.Message());
    }
}

} //namespace nBase