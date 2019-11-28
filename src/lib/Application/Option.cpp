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

} //namespace nApplication