#pragma once

#include <map>
#include <string>
#include "Option.h"

namespace nApplication {

class cOptionParser {
public:
    typedef ::std::map<::std::string, cOption*> tOptions;

public:
    /** Destructor */
    virtual ~cOptionParser();

    /** Constructor */
    cOptionParser();

public:
    /** Parses the commandline options */
    void Parse(int argc, char** argv) const;

    /** Adds a valid option */
    template<typename T> void AddOption(const ::std::string iName, ::nBase::cParser<T>* iParser, T* iValue, const T& iDefaultValue);

private:
    tOptions mOptions;
};

template<typename T>
void
cOptionParser::AddOption(const ::std::string iName, ::nBase::cParser<T>* iParser, T* iValue, const T& iDefaultValue) {
    mOptions[iName] = new cOptionT<T>(iName, iParser, iValue, iDefaultValue);
}

} // namespace nApplication