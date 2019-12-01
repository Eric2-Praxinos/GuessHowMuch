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
    // Set default values for all options before parsing (ensure all options get a valid value even if not present in command line)
    for(tOptions::const_iterator it = mOptions.begin(); it != mOptions.end(); it++) {
        const cOption* option = (*it).second;
        option->SetValueToDefaultValue();
    }

    // For each command line argument 
    for (int i = 1; i < argc; i++) {
        // Find corresponding option
        tOptions::const_iterator it = mOptions.find(argv[i]);
        if (it == mOptions.end()) {
            throw cError("invalid_argument", "Unknown option : " + std::string(argv[i]));
        }
        const cOption* option = (*it).second;
        
        try {
            // If option has a specified value
            if (i+1 < argc) {
                tOptions::const_iterator it2 = mOptions.find(argv[i+1]);
                //If option value is not an other option 
                if (it2 == mOptions.end()) {
                    //Parse the option value
                    option->Parse(argv[i+1]);
                    i++;
                    continue;
                }
            }

            // If option has no specified value, parse an empty value
            option->Parse("");
        } catch (cError iError) {
            throw cError(iError.Code(), option->Name() + " option error : " + iError.Message());
        }
    }
}

} // namespace nApplication