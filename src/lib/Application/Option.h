#pragma once

#include <string>
#include "Parser.h"

namespace nApplication {

class cOption {
public:
    /** Destructor */
    ~cOption();

    /** Constructor */
    cOption(const ::std::string iName);

public:
    /** Get the option name */
    const std::string& Name() const;

    /** Parse the String and set the value */
    virtual void Parse(const ::std::string& iString) = 0;

private:
    ::std::string mName;
};

template <typename T>
class cOptionT : cOption {
public:
    /** Destructor */
    virtual ~cOptionT();

    /** Constructor */
    cOptionT(const ::std::string iName, ::nBase::cParser<T>* iParser, T* iValue);

public:
    /** Sets the value of the option */
    virtual void Parse(const ::std::string& iString);

private:
    T* mValue;
};

} // namespace nApplication