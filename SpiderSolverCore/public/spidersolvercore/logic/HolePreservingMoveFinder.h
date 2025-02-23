#pragma once

#include "spidersolvercore/Model/MoveCombo.h"

#include <vector>

class SpiderTableau;

namespace HolePreservingMoveFinder
{
    std::vector<MoveCombo> AllGoodMoves(const SpiderTableau& tableau);

    std::vector<MoveCombo> FlipRuns(const SpiderTableau& tableau);
    std::vector<MoveCombo> MoveTwoRuns(const SpiderTableau& tableau);
    std::vector<MoveCombo> InsertRun(const SpiderTableau& tableau);
    std::vector<MoveCombo> RemoveMiddleRun(const SpiderTableau& tableau);
    std::vector<MoveCombo> TradeHolesA(const SpiderTableau& tableau);
    std::vector<MoveCombo> TradeHolesB(const SpiderTableau& tableau);
    std::vector<MoveCombo> SwapRuns(const SpiderTableau& tableau);
}
