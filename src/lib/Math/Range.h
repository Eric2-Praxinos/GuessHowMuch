#pragma once

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

    /** Returns wether iValue is included in the range */
    bool Includes(int iValue) const;

private:
    int mMin;
    int mMax;
};

}
