#include "ParserInteger.h"
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
        throw new std::invalid_argument("Invalid or missing value");
    } catch (std::out_of_range excpetion) {
        throw new std::invalid_argument("Out of range value");
    }

    if (mHasRange && !mRange.Includes(integer)) {
        throw new std::out_of_range("Out of range value");
    }
    return integer;
}

} //namespace nBase