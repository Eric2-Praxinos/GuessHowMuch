#include "ParserString.h"

namespace nBase {

cParserString::~cParserString() {
}

cParserString::cParserString() : 
    cParser<::std::string>()
{
}

::std::string
cParserString::Parse(const ::std::string& iString, const ::std::string& iDefaultValue) const {
    return iString;
}

} //namespace nBase