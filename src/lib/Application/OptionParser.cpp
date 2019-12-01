#include "OptionParser.h"
#include "unistd.h"
#include "../Base/Error.h"
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
    for(tOptions::const_iterator it = mOptions.begin(); it != mOptions.end(); it++) {
        const cOption* option = (*it).second;
        option->SetValueToDefaultValue();
    }

    for (int i = 1; i < argc; i++) {
        tOptions::const_iterator it = mOptions.find(argv[i]);
        if (it == mOptions.end()) {
            throw cError("invalid_argument", "Unknown option : " + std::string(argv[i]));
        }
        const cOption* option = (*it).second;
        
        try {
            if (i+1 < argc) {
                tOptions::const_iterator it2 = mOptions.find(argv[i+1]);
                if (it2 == mOptions.end()) {
                    option->Parse(argv[i+1]);
                    i++;
                    continue;
                }
            }

            option->Parse("");
        } catch (cError iError) {
            throw cError(iError.Code(), option->Name() + " option error : " + iError.Message());
        }
    }
}

} // namespace nApplication