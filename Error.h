//
// Created by fperezboco on 18/10/19.
//

#ifndef TP_ERROR_H
#define TP_ERROR_H


#include <exception>
#include <cstdarg>

#define ERROR_MSG_LENGTH 100

/* Handles exceptions and error messages */
class Error : public std::exception {
private:
    char error_msg[ERROR_MSG_LENGTH]{};

public:
    explicit Error(const char * fmt, ...) noexcept;
    const char * what() const noexcept override;
    void printError();
};



#endif //TP_ERROR_H
