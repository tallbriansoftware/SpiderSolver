#include "spidersolvercore/strategy/Strategy.h"

#include "spidersolvercore/logic/HolePreservingMoveFinder.h"
#include "spidersolvercore/logic/SingleMoveFinder.h"
#include "spidersolvercore/model/Ancestry.h"
#include "spidersolvercore/model/SpiderTableau.h"
#include "spidersolvercore/strategy/BoardScorer.h"
#include "spidersolvercore/strategy/ScoredMove.h"
#include "spidersolvercore/strategy/SearchContext.h"

#include <algorithm>


Strategy::Strategy()
    : m_evals(0)
{
    m_boardScorer = std::make_unique<BoardScorer>();
}

Strategy::Strategy(const BoardScorer& boardScorer)
    : m_evals(0)
{
    m_boardScorer = std::make_unique<BoardScorer>(boardScorer);

}

Strategy::~Strategy()
{ }

int Strategy::GetEvals() const
{
    return m_evals;
}

float Strategy::MaxScore() const
{
    return m_boardScorer->MaxScore();
}

float Strategy::ComputeScore(const SpiderTableau& tableau)
{
    m_evals += 1;
    return m_boardScorer->ComputeScore(tableau);
}

std::vector<ScoredMove> Strategy::FindScoredMoves(
    const SpiderTableau& parentTableau,
    const Ancestry& ancestry)
{
    SpiderTableau tableau(parentTableau);
    Ancestry searchAncestry(ancestry);
    SearchContext ctx(4, ancestry);

    auto scoredBoards = FindAndScoreTreeDepthOfChildren(0, ctx, tableau);
    return scoredBoards;
}

const BoardScorer& Strategy::GetBoardScorer() const
{
    return *m_boardScorer;
}

std::vector<ScoredMove> Strategy::GetSortedMoveScores(
    const SearchContext& ctx,
    const SpiderTableau& parentTableau,
    const std::vector<MoveSingle>& smoves)
{
    std::vector<MoveCombo> moves;
    for (auto& smove : smoves)
        moves.push_back(MoveCombo(smove));

    return GetSortedMoveScores(ctx, parentTableau, moves);
}


std::vector<ScoredMove> Strategy::GetSortedMoveScores(
    const SearchContext& ctx,
    const SpiderTableau& parentTableau,
    const std::vector<MoveCombo>& moves)
{
    SpiderTableau tableau(parentTableau);
    std::vector<ScoredMove> movesWithScores;

    for (auto& move : moves)
    {
        SpiderTableau::SavePoint save(tableau);
        tableau.DoMove(move, DoTurnCard::No);
        std::string tabString = tableau.GetTableauString();

        if (!ctx.IsAParentPosition(tabString))
        {
            float boardScore = (float)ComputeScore(tableau);
            movesWithScores.push_back(ScoredMove(boardScore, MoveCombo(move)));
        }
    }

    // Sort the moves by score,  greatest to least.
    std::sort(begin(movesWithScores), end(movesWithScores), [](ScoredMove& a, ScoredMove& b)
        {
            return a.localScore > b.localScore;
        });
    return movesWithScores;
}


namespace
{
    ScoredMove* FindBest(std::vector<ScoredMove> moveswithScores)
    {
        float bestScore = 0.0;
        std::vector<MoveCombo> bestPath;
        ScoredMove* best = nullptr;
        for (int i = 0; i < (int)moveswithScores.size(); i++)
        {
            auto& moveWithScore = moveswithScores[i];
            if (moveWithScore.depthScore > bestScore)
            {
                bestScore = moveWithScore.depthScore;
                best = &moveWithScore;
            }
        }
        return best;
    }
}

namespace
{
    std::vector<MoveCombo> GetMoves(const SpiderTableau& tableau)
    {
        auto moves = SingleMoveFinder::AllMoves(tableau);
        auto holeMoves = HolePreservingMoveFinder::AllMoves(tableau);
        moves.insert(moves.end(), holeMoves.begin(), holeMoves.end());
        return moves;
    }
}


std::vector<ScoredMove> Strategy::FindAndScoreTreeDepthOfChildren(
    int depth,
    SearchContext& ctx,
    const SpiderTableau& parentTableau)
{
    // Preamble code to set things up.
    auto moves = GetMoves(parentTableau);
    if (moves.size() == 0)
        return {};

    SpiderTableau tableau(parentTableau);
    std::string parentTabString = tableau.GetTableauString();
    ctx.AddParentPosition(parentTabString);

    // Look at moves one level down.
    std::vector<ScoredMove> movesWithScores = GetSortedMoveScores(ctx, tableau, moves);
    if (movesWithScores.size() == 0)
        return {};

    //for (auto& scoredMove : movesWithScores)
    //{
    //    SpiderTableau::SavePoint save(tableau);
    //    MoveCombo move = scoredMove.move;
    //    tableau.DoMove(move, DoTurnCard::No);

    //    ScoredMove* result = nullptr;
    //    std::vector<ScoredMove> scoredMoves;

    //    if (depth + 1 < ctx.GetMaxDepth())
    //    {
    //        scoredMoves = FindAndScoreTreeDepthOfChildren(depth + 1, ctx, tableau);
    //        if (scoredMoves.size() > 0)
    //            result = FindBest(scoredMoves);
    //    }
    //    if (result != nullptr)
    //    {
    //        result->futurePath.push_back(move);
    //        scoredMove.depthScore = result->depthScore;
    //        scoredMove.futurePath = result->futurePath;
    //    }
    //    else
    //    {
    //        scoredMove.depthScore = scoredMove.localScore;
    //        scoredMove.futurePath = {};
    //    }
    //}

    ctx.RemoveParentPosition(parentTabString);
    return movesWithScores;
}

