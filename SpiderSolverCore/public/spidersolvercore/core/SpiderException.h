#pragma once

#include <exception>
#include <string>

class SpiderException : public std::exception
{
public:
    SpiderException(char const* message);
    ~SpiderException();
};
