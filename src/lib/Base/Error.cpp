#include "Error.h"
    
cError::~cError() {

}

cError::cError(const std::string& iCode, const std::string& iMessage) :
    mCode(iCode),
    mMessage(iMessage)
{

}

const std::string&
cError::Message() const {
    return mMessage;
}

const std::string&
cError::Code() const {
    return mCode;
}
