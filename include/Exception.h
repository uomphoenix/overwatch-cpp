#ifndef _EXCEPTION_H
#define _EXCEPTION_H

#include <exception>

class PiCamOpenError : public std::exception
{
    virtual const char *what() const throw()
    {
        return "Unable to open Pi Cam for streaming";
    }
};

#endif
