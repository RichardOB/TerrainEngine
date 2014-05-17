#include <iostream>
#include <string>

#include "MessageException.h"

MessageException::
MessageException() throw()
{
}

MessageException::
~MessageException() throw()
{
}

const char* MessageException::
what() const throw()
{
   string message = getPrefix() + ": " + getMessage();

   return message.c_str();
}

