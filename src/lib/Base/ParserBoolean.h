#pragma once

#include "Parser.h"

namespace nBase {

class cParserBoolean : public cParser<bool> {
public:
    /** Destructor */
    ~cParserBoolean();

    /** Constructor */
    cParserBoolean(bool iDefaultValue);

public:
    /** Parse the string */
    virtual bool Parse(const ::std::string& iString, const bool& iDefaultValue) const;

private:
    bool mDefaultValue;
};

} //namespace nBase