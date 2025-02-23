#pragma once

//#include "spiderautocore/model/SpiderStack.h"
//#include "SpiderAutoCore/Model/SpiderTableau.h"

#include <memory>

class SpiderStack;
class SpiderTableau;

class SpiderTableauFactory
{
public:
    static std::shared_ptr<SpiderTableau> Create(int numSuits, int seed);
    static std::shared_ptr<SpiderTableau> Create(SpiderStack& deck);

    static std::shared_ptr<SpiderTableau> CreateAllUp(int numSuits, int seed);
};