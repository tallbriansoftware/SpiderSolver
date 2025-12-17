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

float Strategy::ComputeScore(const SpiderTableau& tableau) const
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


const std::vector<std::string> Strategy::GetModifiedTermNames() const
{
    return m_boardScorer->GetModifiedTermNames();
}

const std::vector<float> Strategy::GetModifiedTerms() const
{
    return m_boardScorer->GetModifiedTerms();
}


std::vector<ScoredMove> Strategy::TreeSearch(
    const SpiderTableau& parentTableau,
    SearchContext& ctx)
{
    auto treeMoves = FindAndScoreToDepth(1, ctx, {}, parentTableau);

    // convert to scored moves
    std::vector<ScoredMove> result;
    for (TreeMove& tm : treeMoves)
    {
        result.push_back(ScoredMove(tm.GetScore(), tm.GetMove(), tm.GetPathDown()));
    }
    return result;
}


std::vector<TreeMove> Strategy::FindAndScoreToDepth(
    int depth,
    SearchContext& ctx,
    std::vector<MoveCombo> pathDown,
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

    pathDown.resize(depth);

    // If we are not at the leaf level, then just expand down.
    for (auto& move : moves)
    {
        pathDown[depth-1] = move;

        SpiderTableau::SavePoint save(tableau);
        tableau.DoMove(move, DoTurnCard::No);

        std::string tabString = tableau.GetTableauString();

        // if we have seen this before as a parent, then skip it.
        if (ctx.IsAParentPosition(tabString))
            continue;

        // check if we have seen this know before in the search.
        // [Direct parents are a special case addressed above.]
        SpiderNode foundSpiderNode;
        TreeMove treeMove;

        if (ctx.TryFindSpiderNode(tabString, foundSpiderNode))
        {
            if (foundSpiderNode.GetDepth() <= depth)
            {
                // If we have seen this position elsewhere in the search, but not
                // a direct parent, then recapture the data we already know.
                treeMove = TreeMove(foundSpiderNode.GetScore(), move, pathDown);
                treeMoves.push_back(treeMove);
                continue;
            }
            // We have seen this node before, but... this is a better
            // candidate for this position because it is at a shallower depth.
            // so remove the deeper node and continue.
            ctx.RemoveSpiderNode(tabString);
        }

        if (depth >= ctx.GetMaxDepth())
        {
            // Are we at maximum depth.  Don't go deeper.
            // Compute Score as a "leaf node"
            float score = (float)ComputeScore(tableau);
            treeMove = TreeMove(score, move, pathDown);
        }
        else
        {
            // Recurse deeper.
            std::vector<TreeMove> childTreeMoves = FindAndScoreToDepth(depth + 1, ctx, pathDown, tableau);
            if (childTreeMoves.size() > 0)
            {
                // If there are children then score is the best of its children.
                TreeMove bestMove = childTreeMoves[0];
                treeMove = TreeMove(bestMove.GetScore(), move, bestMove.GetPathDown());
            }
            else
            {
                // If no children then this nodes score is its score.
                float score = (float)ComputeScore(tableau);
                treeMove = TreeMove(score, move, pathDown);
            }
        }
        treeMoves.push_back(treeMove);
        ctx.AddSpiderNode(SpiderNode(depth, tabString, treeMove));
    }
    SortTreeMoves(treeMoves);
    ctx.RemoveParentPosition(parentTabString);
    return treeMoves;
}

