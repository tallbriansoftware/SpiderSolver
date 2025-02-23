#pragma once

#include <functional>
#include <vector>

class MoveSingle;
class MoveCombo;

class SpiderStack;
class SpiderTableau;


namespace SingleMoveFinder
{
    std::vector<MoveSingle> AllSingleMoves(const SpiderTableau& tableau);
    std::vector<MoveCombo> AllSingleMovesAsCombos(const SpiderTableau& tableau);

    int AddSimpleMoves(std::vector<MoveSingle>& moves, const SpiderTableau& tableau);
    int AddColorUpMoves(std::vector<MoveSingle>& moves, const SpiderTableau& tableau);
    int AddSuitedRankSplittingMoves(std::vector<MoveSingle>& moves, const SpiderTableau& tableau);
    int AddHoleFillingMoves(std::vector<MoveSingle>& moves, const SpiderTableau& tableau);
}
