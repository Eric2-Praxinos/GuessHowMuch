#include "OptionParser.h"
#include "unistd.h"
#include <stdexcept>

namespace nApplication {

/** Destructor */
cOptionParser::~cOptionParser() {
    tOptions::iterator it;
    for(it = mOptions.begin(); it != mOptions.end(); it++) {
        delete (*it).second;
    }
}

/** Constructor */
cOptionParser::cOptionParser() :
    mOptions()
{
}

void
cOptionParser::Parse(int argc, char** argv) const {
    for (int i = 0; i < argc; i++) {
        tOptions::const_iterator it = mOptions.find(argv[i]);
        if (it == mOptions.end()) {
            throw new std::invalid_argument("Unknown argument");
        }
        const cOption* option = (*it).second;
        option->SetValueToDefaultValue();

        if (i+1 < argc) {
            tOptions::const_iterator it2 = mOptions.find(argv[i+1]);
            if (it == mOptions.end()) {
                option->Parse(argv[i+1]);
                i++;
                continue;
            }
        }

        option->Parse("");
    }
}

} // namespace nApplication