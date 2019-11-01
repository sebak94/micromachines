//
// Created by fperezboco on 18/10/19.
//

#include "Error.h"
#include <cstdio>
#include <iostream>

/* Creates an exception receiving multiple arguments of
 * different formats to form a detailed string message */
Error::Error(const char *fmt, ...) noexcept {
    va_list args;

    va_start(args, fmt);
    vsnprintf(error_msg, ERROR_MSG_LENGTH, fmt, args);
    va_end(args);
}

/* Gets formatted error message */
const char * Error::what() const noexcept {
    return error_msg;
}

void Error::printError() {
    std::cout << "Error: " << this->what() << std::endl;
}