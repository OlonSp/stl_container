#include <iostream>
#include "Exceptions.h"

using namespace std;

OutOfRangeException::OutOfRangeException(const string& error)
{
    errorMessage = "!OUT OF RANGE ERROR at " + error;
}

const char* OutOfRangeException::what() const noexcept
{
    return errorMessage.c_str();
}

IteratorException::IteratorException(const string& error)
{
    errorMessage = "!ITERATOR ERROR at " + error;
}

const char* IteratorException::what() const noexcept
{
    return errorMessage.c_str();
}
