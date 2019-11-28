#pragma once

#include <map>
#include <string>
#include "Option.h"
#include "Parser.h"

namespace nApplication {

class cOptionParser {
public:
    /** Destructor */
    virtual ~cOptionParser();

    /** Constructor */
    cOptionParser();

public:
    /** Parses the commandline options */
    void Parse(int argc, char** argv) const;

    /** Adds a valid option */
    template<typename T> void AddOption(const ::std::string iName, ::nBase::cParser<T>* iParser, T* iValue);

private:
    ::std::map<::std::string, cOption*> mOptions;
};

template<typename T>
void
cOptionParser::AddOption(const ::std::string iName, ::nBase::cParser<T>* iParser, T* iValue) {
    mOptions.insert(::std::pair<std::string, cOption*>(iName, new cOptionT<T>(iName, iParser, iValue))
}


} // namespace nApplication