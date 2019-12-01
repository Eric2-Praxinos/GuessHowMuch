#pragma once

#include <QtCore/QObject>

namespace nClient {
namespace nApplication {

class cSession;

class cController : public QObject {
Q_OBJECT
public:
    /** Destructor */
    virtual ~cController() = 0;

    /** Constructor */
    cController(cSession* iSession);

public:
    /** Returns the session */
    cSession* Session();

private:
    cSession* mSession;
};

}
}
