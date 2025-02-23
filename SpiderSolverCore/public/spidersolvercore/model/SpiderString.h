#pragma once

class SpiderStack;
class SpiderTableau;

#include <string>

namespace SpiderString
{
    std::string Create(const SpiderStack& stack);
    std::string Create(const SpiderTableau& tableau);
}