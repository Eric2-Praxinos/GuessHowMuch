#pragma once

#include "Parser.h"
#include "../Math/Range.h"

namespace nBase {
    
class cParserInteger : public cParser<int> {
public:
    /** Destructor */
    ~cParserInteger();

    /** Constructor */
    cParserInteger();

    /** Constructor */
    cParserInteger(const ::nMath::cRange& iRange);

public:
    /** Parse the string */
    virtual int Parse(const ::std::string& iString, const int& iDefaultValue) const;

private:
    ::nMath::cRange mRange;
    bool mHasRange;
};

} //namespace nBase