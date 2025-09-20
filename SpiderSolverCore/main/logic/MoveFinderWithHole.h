#pragma once

#include "spidersolvercore/Model/MoveCombo.h"

#include <vector>

class SpiderTableau;

namespace MoveFinderWithHole
{
    std::vector<MoveCombo> GetAllOneHoleMoves(const SpiderTableau& tableau);
    int AddAllOneHoleMoves(std::vector<MoveCombo>& moves, const SpiderTableau& tableau);

    int AddMoveTwoRuns(std::vector<MoveCombo>& moves, const SpiderTableau& tableau);
    int AddFlipRuns(std::vector<MoveCombo>& moves, const SpiderTableau& tableau);
    int AddInsertRun(std::vector<MoveCombo>& moves, const SpiderTableau& tableau);
    int AddRemoveMiddleRun(std::vector<MoveCombo>& moves, const SpiderTableau& tableau);
    int AddTradeHolesA(std::vector<MoveCombo>& moves, const SpiderTableau& tableau);
    int AddTradeHolesB(std::vector<MoveCombo>& moves, const SpiderTableau& tableau);
    int AddSwapRuns(std::vector<MoveCombo>& moves, const SpiderTableau& tableau);
}
