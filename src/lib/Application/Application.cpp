#include "Application.h"
#include "OptionParser.h"

namespace nApplication {

/** Destrutor */
cApplication::~cApplication() {
    delete mOptionParser;
};

/** Constructor */
cApplication::cApplication() : 
    mOptionParser(new cOptionParser())
{
}

const cOptionParser*
cApplication::OptionParser() const {
    return mOptionParser;
}

cOptionParser*
cApplication::OptionParser() {
    return mOptionParser;
}

} // namespace nApplication