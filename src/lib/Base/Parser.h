#pragma once

#include <string>

namespace nBase {

template <typename T>
class cParser {
public:
    /** Destructor */
    virtual ~cParser();

    /** Constructor */
    cParser();

public:
    /** Parse the string */
    virtual T Parse(const ::std::string& iString, const T& iDefaultValue) const = 0;
};

template <typename T>
cParser<T>::~cParser() {

}

template <typename T>
cParser<T>::cParser() {

}

} //namespace nBase