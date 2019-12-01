#pragma once

#include <QtCore/QString>

namespace nShared {
namespace nSession {

enum eStatus {
    kInProgress = 0,
    kSuccess = 1,
    kFailure = 2,
    kAbandoned = 3,
    kError = 4
};

QString StatusName (eStatus iStatus);

}
}
