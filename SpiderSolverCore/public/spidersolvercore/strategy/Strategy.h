#pragma once

#include "spidersolvercore/strategy/ScoredMove.h"

#include <memory>

class Ancestry;
class SpiderTableau;
class BoardScorer;
class SearchContext;
class TreeMove;
class TreeContext;
class MoveCombo;


class Strategy
{
public:
    Strategy();
    Strategy(const BoardScorer& boardScorer);
    ~Strategy();

    int GetEvals() const;
    void ClearEvals();

    float MaxScore() const;

    float ComputeScore(const SpiderTableau& tableau);

    std::vector<ScoredMove> FindScoredMoves(
        const SpiderTableau& tableau,
        const Ancestry& ancestry,
        int depth);

    const BoardScorer& GetBoardScorer() const;

private:

    std::vector<ScoredMove> TreeSearch(
        const SpiderTableau& parentTableau,
        SearchContext& ctx);

    std::vector<TreeMove> FindAndScoreToDepth(
        int depth,
        SearchContext& ctx,
        const SpiderTableau& parentTableau);

private:
    std::unique_ptr<BoardScorer> m_boardScorer;
    int m_evals;
};
