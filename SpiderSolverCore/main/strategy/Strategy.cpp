#include "spidersolvercore/strategy/Strategy.h"

#include "spidersolvercore/logic/MoveFinder.h"
#include "spidersolvercore/model/Ancestry.h"
#include "spidersolvercore/model/SpiderTableau.h"
#include "spidersolvercore/model/SpiderNode.h"
#include "spidersolvercore/strategy/BoardScorer.h"
#include "spidersolvercore/strategy/ScoredMove.h"
#include "spidersolvercore/strategy/SearchContext.h"
#include "spidersolvercore/strategy/TreeMove.h"

#include <algorithm>
#include <memory>

#include <assert.h>


Strategy::Strategy()
    : m_boardScorer(std::make_unique<BoardScorer>())
    , m_evals(0)
{
}

Strategy::Strategy(const BoardScorer& boardScorer)
    : m_boardScorer(std::make_unique<BoardScorer>(boardScorer))
    , m_evals(0)
{
}

Strategy::Strategy(const Strategy& strategy)
    : m_boardScorer(std::make_unique<BoardScorer>(*strategy.m_boardScorer))
    , m_evals(strategy.m_evals)
{
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

namespace
{
    void SortTreeMoves(std::vector<TreeMove>& treeMoves)
    {
        // Sort the moves by score,  greatest to least.
        std::sort(begin(treeMoves), end(treeMoves), [](TreeMove& a, TreeMove& b)
            {
                return a.GetScore() > b.GetScore();
            });
    }
}


std::vector<ScoredMove> Strategy::FindScoredMoves(
    MoveFinderFunc moveFinder,
    const SpiderTableau& parentTableau,
    const Ancestry& ancestry,
    int depth)
{
    int depthLimit = depth;
    Ancestry searchAncestry(ancestry);
    SearchContext ctx(depthLimit, ancestry, moveFinder);

    auto scoredMoves = TreeSearch(parentTableau, ctx);
    return scoredMoves;
}


const BoardScorer& Strategy::GetBoardScorer() const
{
    return *m_boardScorer;
}


std::vector<ScoredMove> Strategy::TreeSearch(
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
    auto moves = ctx.GetMoves(parentTableau);
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

        // If we have seen this position elsewhere in the search, but not
        // a direct parent, then recapture the data we already know.
        SpiderNode foundSpiderNode;
        bool done = false;
        if (ctx.TryFindSpiderNode(tabString, foundSpiderNode))
        {
            if (foundSpiderNode.GetDepth() <= depth)
            {
                treeMoves.push_back(TreeMove(foundSpiderNode.GetScore(), move));
                done = true;
            }
            else
            {
                // We have a better candidate for this position
                // because it is at a shallower depth.
                ctx.RemoveSpiderNode(tabString);
            }
        }
        // This is not a previously seen position.
        // Continue evaluating and store the positions.
        if(!done)
        {
            float score = -1.0;

            // Are we at maximum depth.  Don't go deeper.
            if (depth >= ctx.GetMaxDepth())
            {
                score = (float)ComputeScore(tableau);
            }
            else
            {
                // Recurse deeper.
                auto childTreeMoves = FindAndScoreToDepth(depth + 1, ctx, tableau);
                // If there were no moves treat it as a leaf node.  (no best child)
                if (childTreeMoves.size() == 0)
                {
                    score = (float)ComputeScore(tableau);
                }
                // On return note the move to the best child and best child's score.
                else
                {
                    auto& bestChild = childTreeMoves[0];
                    score = bestChild.GetScore();
                }
            }
            treeMoves.push_back(TreeMove(score, move));
            ctx.AddSpiderNode(SpiderNode(depth, tabString, score));
        }
    }
    SortTreeMoves(treeMoves);
    ctx.RemoveParentPosition(parentTabString);
    return treeMoves;
}

