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
    Strategy(const Strategy& strategy);
    Strategy(const BoardScorer& boardScorer);
    ~Strategy();

    int GetEvals() const;
    void ClearEvals();

    float MaxScore() const;

    float ComputeScore(const SpiderTableau& tableau) const;

    std::vector<ScoredMove> FindScoredMoves(
        MoveFinderFunc moveFinder,
        const SpiderTableau& tableau,
        const Ancestry& ancestry,
        int depth);

    const std::vector<std::string> GetModifiedTermNames() const;
    const std::vector<float> GetModifiedTerms() const;

private:

    std::vector<ScoredMove> TreeSearch(
        const SpiderTableau& parentTableau,
        SearchContext& ctx);

    std::vector<TreeNode> FindAndScoreToDepth(
        int depth,
        SearchContext& ctx,
        std::vector<MoveCombo> movePath,
        const SpiderTableau& parentTableau);

private:
    std::unique_ptr<BoardScorer> m_boardScorer;
    mutable int m_evals;
};
