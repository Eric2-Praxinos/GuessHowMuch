#pragma once

#include "Parser.h"
#include "ParserInteger.h"
#include "../Math/Range.h"

namespace nBase {
    
class cParserInterval : public cParser<::nMath::cRange> {
public:
    /** Destructor */
    ~cParserInterval();

    /** Constructor */
    cParserInterval();

public:
    /** Parse the string */
    virtual ::nMath::cRange Parse(const ::std::string& iString, const ::nMath::cRange& iDefaultValue) const;

private:
    cParserInteger* mParserInteger;
};

} //namespace nBase