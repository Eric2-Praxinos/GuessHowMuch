#include "ParserInteger.h"
#include "Error.h"
#include <stdexcept>

namespace nBase {

cParserInteger::~cParserInteger() {
}

cParserInteger::cParserInteger() : 
    cParser<int>(),
    mRange(),
    mHasRange(false)
{
}

cParserInteger::cParserInteger(const ::nMath::cRange& iRange) : 
    cParser<int>(),
    mRange(iRange),
    mHasRange(true)
{
}

int
cParserInteger::Parse(const ::std::string& iString, const int& iDefaultValue) const {
    int integer = iDefaultValue;
    try {
        integer = std::stoi(iString);
    } catch (std::invalid_argument exception) {
        throw cError("invalid_argument", "Value is missing or is not a valid integer value");
    } catch (std::out_of_range excpetion) {
        throw cError("out_of_range", "Value is outside of acceptable range for an integer value");
    }

    if (mHasRange && !mRange.Includes(integer)) {
        throw cError("out_of_range", "Value is outside the expected range");
    }
    return integer;
}

} //namespace nBase