#pragma once

#include <functional>
#include <vector>

class MoveSingle;
class MoveCombo;

class SpiderStack;
class SpiderTableau;


namespace SingleMoveFinder
{
    std::vector<MoveCombo> AllMoves(const SpiderTableau& tableau);

    std::vector<MoveSingle> AllMoveSingles(const SpiderTableau& tableau);
    int AddSimpleMoves(std::vector<MoveSingle>& moves, const SpiderTableau& tableau);
    int AddColorUpMoves(std::vector<MoveSingle>& moves, const SpiderTableau& tableau);
    int AddSuitedRankSplittingMoves(std::vector<MoveSingle>& moves, const SpiderTableau& tableau);
    int AddHoleFillingMoves(std::vector<MoveSingle>& moves, const SpiderTableau& tableau);
}
