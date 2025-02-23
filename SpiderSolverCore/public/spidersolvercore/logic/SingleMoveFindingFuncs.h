#pragma once

#include <functional>
#include <vector>

class MoveSingle;
class MoveCombo;

class SpiderStack;
class SpiderTableau;

namespace SingleMoveFindingFuncs
{
    using FindingFunction = std::function<int(const SpiderStack&, const SpiderStack&)>;

    int FindMoves(
        std::vector<MoveSingle>& moves,
        const SpiderTableau& tableau,
        SingleMoveFindingFuncs::FindingFunction findingFunc);

    int FindMovesForDestinationStack(
        std::vector<MoveSingle>& moves,
        const SpiderStack& destStack,
        const SpiderTableau& tableau,
        SingleMoveFindingFuncs::FindingFunction findingFunc);

    // List returning methods.
    //std::vector<MoveSingle> Find(const SpiderTableau& tableau, FindingFunction func);

    int SimpleMove(const SpiderStack& src, const SpiderStack& dest);
    int ColorUpMove(const SpiderStack& src, const SpiderStack& dest);
    int SuitedRunSplittingMove(const SpiderStack& src, const SpiderStack& dest);
    int HoleFillingMove(const SpiderStack& src, const SpiderStack& dest);
}