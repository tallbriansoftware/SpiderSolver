#pragma once

#include "spidersolvercore/logic/MoveFinderFunc.h"

#include <memory>
#include <vector>
#include <functional>
#include <string>

class Ancestry;
class SpiderTableau;
class BoardScorer;
class SearchContext;
class TreeNode;
class TreeContext;
class MoveCombo;
class ScoredMove;


class Strategy
{
public:
    Strategy();
    ~Strategy();

    int GetEvals() const;
    void ClearEvals();

    float MaxScore() const;

    float ComputeScore(const SpiderTableau& tableau) const;

    std::vector<ScoredMove> FindScoredMoves(
        MoveFinderFunc moveFinder,
        std::vector<MoveCombo>& disregardedMoves,
        const SpiderTableau& tableau,
        const Ancestry& ancestry,
        int depth);

private:

    std::vector<ScoredMove> TreeSearch(
        const SpiderTableau& parentTableau,
        std::vector<MoveCombo>& disregardedMoves,
        SearchContext& ctx);

    std::vector<TreeNode> FindAndScoreToDepth(
        int depth,
        SearchContext& ctx,
        std::vector<MoveCombo> movePath,
        const SpiderTableau& parentTableau);

private:
    std::unique_ptr<BoardScorer> m_boardScorer;
    mutable std::vector<MoveCombo> m_topLevelDisregardedMoves;
    mutable int m_evals;
};
