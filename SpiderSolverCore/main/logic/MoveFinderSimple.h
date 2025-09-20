#pragma once

#include <functional>
#include <vector>

class MoveSingle;
class MoveCombo;
class SpiderTableau;
class SpiderStack;

namespace MoveFinderSimple
{
    int AddSimpleMoves(std::vector<MoveSingle>& moves, const SpiderTableau& tableau);
    int AddColorUpMoves(std::vector<MoveSingle>& moves, const SpiderTableau& tableau);
    int AddHoleFillingMoves(std::vector<MoveSingle>& moves, const SpiderTableau& tableau);
    int AddRunSplittingMoves(std::vector<MoveSingle>& moves, const SpiderTableau& tableau);
}
