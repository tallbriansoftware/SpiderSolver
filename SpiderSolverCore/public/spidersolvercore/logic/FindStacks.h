#pragma once

#include "spidersolvercore/Model/CardEnums.h"

#include <vector>

class SpiderTableau;
class SpiderStack;

enum class Exactly { No, Yes };
enum class Sequential { No, Yes };

namespace FindStacks
{
    std::vector<int> ThatWillRecieveRank(Rank rank, const SpiderTableau& tableau);

    // RunPattern (tabeau)
    // Collect the stack Numbers that pass
    // RunPattern(stack) test.
    std::vector<int> RunPattern(
        const SpiderTableau& tableau,
        int numberOfRuns,
        Exactly exactly=Exactly::No,
        Sequential seq=Sequential::No);

    // RunPattern (stack)
    // Returns true/false if the stack has a
    // requested number of suited runs.
    // At least or exactly.
    bool RunPattern(
        const SpiderStack& stack,
        int numberOfRuns,
        Exactly exactly = Exactly::No,
        Sequential seq = Sequential::No);
}