#include "Range.h"
#include <algorithm>

namespace nMath {

cRange::~cRange() {

}

cRange::cRange() : 
    mMin(0),
    mMax(0)
{
}

cRange::cRange(int iBound1, int iBound2) : 
    mMin(std::min(iBound1, iBound2)),
    mMax(std::max(iBound1, iBound2))
{
}

int
cRange::Min() const {
    return mMin;
}

int
cRange::Max() const {
    return mMax;
}

void
cRange::Min(int iMin) {
    mMin = iMin;
}

void
cRange::Max(int iMax) {
    mMax = iMax;
}

bool
cRange::Includes(int iValue) const {
    return iValue >= mMin && iValue <= mMax;
}

QJsonObject
cRange::ToJson() const {
    QJsonObject json;
    json["min"] = mMin;
    json["max"] = mMax;

    return json;
}

}
