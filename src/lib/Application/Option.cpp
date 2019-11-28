#include "Option.h"

namespace nApplication {

cOption::~cOption() {

}

cOption::cOption(const ::std::string iName) :
    mName(iName)
{
}

const std::string&
cOption::Name() const {
    return mName;
}

template<typename T>
cOptionT<T>::~cOptionT() {
    
}

template<typename T>
cOptionT<T>::cOptionT(const ::std::string iName, ::nBase::cParser<T>* iParser, T* iValue) :
    cOption(iName),
    mParser(iParser),
    mValue(iValue)
{
}

template<typename T>
void
cOptionT<T>::Parse(const ::std::string& iString) {
    *mValue = mParser->Parse(iString);
}

} //namespace nApplication