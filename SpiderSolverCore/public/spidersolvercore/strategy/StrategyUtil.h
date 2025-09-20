#pragma once

#include <vector>

class Ancestry;
class MoveSingle;
class MoveCombo;
class ScoredMove;
class SpiderTableau;
class Strategy;

namespace StrategyUtil
{
    void ResortTiedBestMoves(
        std::vector<ScoredMove>& moves,
        const Strategy& strategy,
        const SpiderTableau& tableau);
}