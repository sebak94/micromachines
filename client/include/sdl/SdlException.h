#ifndef __SDLEXCEPTION_H__
#define __SDLEXCEPTION_H__

#include <string>

class SdlException : public std::exception {
public:
    SdlException(const char* description, const char* sdlError);
    const char* what() const noexcept;

private:
    std::string description;
};

#endif
