#include "spidersolvercore/strategy/Strategy.h"

#include "spidersolvercore/logic/HolePreservingMoveFinder.h"
#include "spidersolvercore/logic/MoveFinder.h"
#include "spidersolvercore/model/Ancestry.h"
#include "spidersolvercore/model/SpiderTableau.h"
#include "spidersolvercore/strategy/BoardScorer.h"
#include "spidersolvercore/strategy/ScoredMove.h"
#include "spidersolvercore/strategy/SearchContext.h"
#include "spidersolvercore/strategy/TreeMove.h"

#include <algorithm>

#include <assert.h>


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

void Strategy::ClearEvals()
{
    m_evals = 0;
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
    const Ancestry& ancestry,
    int depth)
{
    int depthLimit = depth;
    Ancestry searchAncestry(ancestry);
    SearchContext ctx(depthLimit, ancestry);

    auto scoredBoards = IterativelyDeepen(parentTableau, ctx);
    return scoredBoards;
}

const BoardScorer& Strategy::GetBoardScorer() const
{
    return *m_boardScorer;
}


namespace
{
    std::vector<MoveCombo> GetMoves(const SpiderTableau& tableau)
    {
        auto moves = MoveFinder::AllSimpleMoves(tableau);
        auto holeMoves = HolePreservingMoveFinder::AllMoves(tableau);
        moves.insert(moves.end(), holeMoves.begin(), holeMoves.end());
        return moves;
    }

    void SortTreeMoves(std::vector<TreeMove>& treeMoves)
    {
        // Sort the moves by score,  greatest to least.
        std::sort(begin(treeMoves), end(treeMoves), [](TreeMove& a, TreeMove& b)
            {
                return a.GetScore() > b.GetScore();
            });
    }
}


std::vector<ScoredMove> Strategy::IterativelyDeepen(
    const SpiderTableau& parentTableau,
    SearchContext& ctx)
{
    auto treeMoves = FindAndScoreToDepth(1, ctx, parentTableau);

    // convert to scored moves
    std::vector<ScoredMove> result;
    for (auto& tm : treeMoves)
    {
        result.push_back(ScoredMove(tm.GetScore(), tm.GetMove()));
    }
    return result;
}


std::vector<TreeMove> Strategy::FindAndScoreToDepth(
    int depth,
    SearchContext& ctx,
    const SpiderTableau& parentTableau)
{
    // Get the child moves
    auto moves = GetMoves(parentTableau);
    if (moves.size() == 0)
        return {};

    // Put the current position in the history (prevent search loops)
    std::string parentTabString = parentTableau.GetTableauString();
    ctx.AddParentPosition(parentTabString);

    SpiderTableau tableau(parentTableau);
    std::vector<TreeMove> treeMoves;

    // If we are not at the leaf level, then just expand down.
    for (auto& move : moves)
    {
        SpiderTableau::SavePoint save(tableau);
        tableau.DoMove(move, DoTurnCard::No);
        std::string tabString = tableau.GetTableauString();

        // if we have seen this before as a parent, then skip it.
        if (ctx.IsAParentPosition(tabString))
            continue;

        if (depth >= ctx.GetMaxDepth())
        {
            float score = (float)ComputeScore(tableau);
            treeMoves.push_back(TreeMove(score, move));
        }
        else
        {
            auto childTreeMoves = FindAndScoreToDepth(depth + 1, ctx, tableau);
            if (childTreeMoves.size() == 0)
            {
                float score = (float)ComputeScore(tableau);
                treeMoves.push_back(TreeMove(score, move));
            }
            else
            {
                auto& bestChild = childTreeMoves[0];
                treeMoves.push_back(TreeMove(move, bestChild));
            }
        }
    }
    SortTreeMoves(treeMoves);
    ctx.RemoveParentPosition(parentTabString);
    return treeMoves;
}

