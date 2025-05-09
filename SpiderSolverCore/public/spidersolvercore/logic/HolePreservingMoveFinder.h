#pragma once

#include "spidersolvercore/Model/MoveCombo.h"

#include <vector>

class SpiderTableau;

namespace HolePreservingMoveFinder
{
    std::vector<MoveCombo> AllMoves(const SpiderTableau& tableau);

    int AddMoveTwoRuns(std::vector<MoveCombo>& moves, const SpiderTableau& tableau);
    int AddFlipRuns(std::vector<MoveCombo>& moves, const SpiderTableau& tableau);
    int AddInsertRun(std::vector<MoveCombo>& moves, const SpiderTableau& tableau);
    int AddRemoveMiddleRun(std::vector<MoveCombo>& moves, const SpiderTableau& tableau);
    int AddTradeHolesA(std::vector<MoveCombo>& moves, const SpiderTableau& tableau);
    int AddTradeHolesB(std::vector<MoveCombo>& moves, const SpiderTableau& tableau);
    int AddSwapRuns(std::vector<MoveCombo>& moves, const SpiderTableau& tableau);
}
