#include "spidersolvercore/core/SpiderException.h"

SpiderException::SpiderException(char const* message)
    : exception(message)
{ }

SpiderException::~SpiderException() = default;