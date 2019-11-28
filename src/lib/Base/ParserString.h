#pragma once

#include <string>
#include "Parser.h"

namespace nBase {

class cParserString : public cParser<::std::string> {
public:
    /** Destructor */
    ~cParserString();

    /** Constructor */
    cParserString();

public:
    /** Parse the string */
    virtual ::std::string Parse(const ::std::string& iString, const ::std::string& iDefaultValue) const;
};

} //namespace nBase