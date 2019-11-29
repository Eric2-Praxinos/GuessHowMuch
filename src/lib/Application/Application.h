#pragma once

#include <QtCore/QObject>

namespace nApplication { 

class cOptionParser;

class cApplication : public QObject {
Q_OBJECT
public:
    /** Destructor */
    virtual ~cApplication() = 0;

    /** Constructor */
    cApplication();

public:
    /** Returns the application OptionParser (const) */
    const cOptionParser* OptionParser() const;

    /** Returns the application OptionParser */
    cOptionParser* OptionParser();

private:
    cOptionParser* mOptionParser;
};

} // namespace nApplication