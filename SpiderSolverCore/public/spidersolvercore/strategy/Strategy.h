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

    float MaxScore() const;

    float ComputeScore(const SpiderTableau& tableau);

    std::vector<ScoredMove> FindScoredMoves(
        const SpiderTableau& tableau,
        const Ancestry& ancestry);

    const BoardScorer& GetBoardScorer() const;

private:
    std::vector<ScoredMove> GetSortedMoveScores(
        const SearchContext& ctx,
        const SpiderTableau& parentTableau,
        const std::vector<MoveSingle>& moves);

    std::vector<ScoredMove> GetSortedMoveScores(
        const SearchContext& ctx,
        const SpiderTableau& parentTableau,
        const std::vector<MoveCombo>& moves);

    std::vector<ScoredMove> FindAndScoreTreeDepthOfChildren(
        int depth,
        SearchContext& ctx,
        const SpiderTableau& parentTableau);

    std::vector<ScoredMove> IterativelyDeepen(
        const SpiderTableau& parentTableau,
        const Ancestry& ancestry);

    std::vector<TreeMove> FindAndScoreToDepth(
        int depth,
        SearchContext& ctx,
        const SpiderTableau& parentTableau);

    TreeMove CreateLeafMove(
        const SpiderTableau& tableau,
        const MoveCombo& move);

    TreeMove CreateBranchMove(
        const MoveCombo& move,
        const TreeMove& bestChild);



private:
    std::unique_ptr<BoardScorer> m_boardScorer;
    int m_evals;
};
