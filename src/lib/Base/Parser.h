#pragma once

#include <string>

namespace nBase {

template <typename T>
class cParser {
public:
    /** Destructor */
    virtual ~cParser() = 0;

    /** Constructor */
    cParser();

public:
    /** Parse the string */
    virtual const T& Parse(const ::std::string& iString) const = 0;
};

} //namespace nBase