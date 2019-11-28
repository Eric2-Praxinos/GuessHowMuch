#pragma once

#include <string>
#include "../Base/Parser.h"

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
    virtual void Parse(const ::std::string& iString) const = 0;

    /** Sets the value to default */
    virtual void SetValueToDefaultValue() const = 0;

private:
    ::std::string mName;
};

template <typename T>
class cOptionT : public cOption {
public:
    /** Destructor */
    virtual ~cOptionT();

    /** Constructor */
    cOptionT(const ::std::string iName, ::nBase::cParser<T>* iParser, T* iValue, const T& iDefaultValue);

public:
    /** Sets the value of the option */
    virtual void Parse(const ::std::string& iString) const;

    /** Sets the value to default */
    virtual void SetValueToDefaultValue() const;

private:
    T* mValue;
    ::nBase::cParser<T>* mParser;
    T mDefaultValue;
};

template<typename T>
cOptionT<T>::~cOptionT() {
    delete mParser;
}

template<typename T>
cOptionT<T>::cOptionT(const ::std::string iName, ::nBase::cParser<T>* iParser, T* iValue, const T& iDefaultValue) :
    cOption(iName),
    mParser(iParser),
    mValue(iValue),
    mDefaultValue(iDefaultValue)
{
}

template<typename T>
void
cOptionT<T>::Parse(const ::std::string& iString) const {
    *mValue = mParser->Parse(iString, mDefaultValue);
}

template<typename T>
void
cOptionT<T>::SetValueToDefaultValue() const {
    *mValue = mDefaultValue;
}

} // namespace nApplication