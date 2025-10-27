#pragma once

#include "spidersolvercore/logic/MoveFinderFunc.h"

#include <memory>
#include <vector>
#include <functional>

class Ancestry;
class SpiderTableau;
class BoardScorer;
class SearchContext;
class TreeMove;
class TreeContext;
class MoveCombo;
class ScoredMove;


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
        MoveFinderFunc moveFinder,
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
