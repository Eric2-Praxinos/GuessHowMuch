#include <string>

class cError {
public:
    /** Destructor */
    ~cError();

    /** Constructor */
    cError(const std::string& iCode, const std::string& iMessage);

public:
    const std::string& Message() const;
    const std::string& Code() const;

private:
    std::string mMessage;
    std::string mCode;
};