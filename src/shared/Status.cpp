#include "Status.h"

namespace nShared {
namespace nSession {

QString StatusName (eStatus iStatus) {
    switch(iStatus) {
        case kInProgress: {
            return "In progress";
        }
        break;
        case kSuccess: {
            return "Succeeded";
        }
        break;
        case kFailure: {
            return "Failed";
        }
        break;
        case kAbandoned: {
            return "Abandoned";
        }
        break;
        case kError: {
            return "Error";
        }
        break;
    }
}

}
}
