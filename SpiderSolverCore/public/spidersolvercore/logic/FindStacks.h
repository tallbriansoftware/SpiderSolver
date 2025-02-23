#pragma once

#include "spidersolvercore/Model/CardEnums.h"

#include <vector>

class SpiderTableau;

enum class Exactly { No, Yes };
enum class Sequential { No, Yes };

namespace FindStacks
{
    std::vector<int> ThatWillRecieveRank(Rank rank, const SpiderTableau& tableau);
    std::vector<int> RunThatWontMakeAHole(const SpiderTableau& tableau);

    std::vector<int> Runs(
        const SpiderTableau& tableau,
        int numberOfRuns,
        Exactly exactly=Exactly::No,
        Sequential seq=Sequential::No);
}