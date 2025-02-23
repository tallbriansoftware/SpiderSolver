#pragma once

#include <vector>

class Ancestry;
class MoveSingle;
class MoveCombo;
struct ScoredMove;
class SpiderTableau;

namespace StrategyUtil
{
    void SortLocalMoves(std::vector<ScoredMove>& moves);

    std::vector<MoveCombo> RemoveRepeats(
        const std::vector<MoveCombo>& moves,
        const SpiderTableau& parentTableau,
        const Ancestry& ancestry);

    std::vector<MoveSingle> RemoveRepeats(
        const std::vector<MoveSingle>& moves,
        const SpiderTableau& parentTableau,
        const Ancestry& ancestry);
}