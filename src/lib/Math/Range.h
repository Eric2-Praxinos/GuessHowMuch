#pragma once

#include <QtCore/QJsonObject>

namespace nMath {

class cRange {
public:
    /** Destructor */
    ~cRange();

    /** Constructor */
    cRange();

    /** Constructor */
    cRange(int iBound1, int iBound2);

public:
    /** Minimum bound of the range */
    int Min() const;

    /** Maximum bound of the range */
    int Max() const;

    /** Set Minimum bound of the range */
    void Min(int iMin);

    /** Set Maximum bound of the range */
    void Max(int iMax);

    /** Returns wether iValue is included in the range */
    bool Includes(int iValue) const;

    /** Returns the json representation */
    QJsonObject ToJson() const;

private:
    int mMin;
    int mMax;
};

}
