#pragma once

#include <memory>

class SpiderStack;

class SpiderDeckFactory
{
public:
    static std::shared_ptr<SpiderStack> Create(int numSuits);
    static std::shared_ptr<SpiderStack> Create(int numSuits, int seed);
};
