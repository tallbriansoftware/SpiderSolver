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
#include <iostream>

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
    void SortTreeNodes(std::vector<TreeNode>& treeNodes)
    {
        // Sort the moves by score,  greatest to least.
        std::sort(begin(treeNodes), end(treeNodes), [](TreeNode& a, TreeNode& b)
            {
                return a.GetScore() > b.GetScore();
            });
    }

    void SortScoredMoves(std::vector<ScoredMove>& scoredMoves)
    {
        // Sort the moves by score,  greatest to least.
        std::sort(begin(scoredMoves), end(scoredMoves), [](ScoredMove& a, ScoredMove& b)
            {
                return a.GetScore() > b.GetScore();
            });
    }
}


std::vector<ScoredMove> Strategy::FindScoredMoves(
    MoveFinderFunc moveFinder,
    std::vector<MoveCombo>& disregardedMoves,
    const SpiderTableau& parentTableau,
    const Ancestry& ancestry,
    int depthLimit)
{
    disregardedMoves.clear();
    Ancestry searchAncestry(ancestry);
    SearchContext ctx(depthLimit, searchAncestry, moveFinder);

    auto scoredMoves = TreeSearch(parentTableau, disregardedMoves, ctx);
    return scoredMoves;
}

std::vector<ScoredMove> Strategy::FindScoredMoves(
    const std::vector<MoveCombo>& firstMoves,
    std::vector<MoveCombo>& disregardedMoves,
    MoveFinderFunc moveFinder,
    const SpiderTableau& parentTableau,
    const Ancestry& ancestry,
    int depthLimit)
{
    disregardedMoves.clear();
    Ancestry searchAncestry(ancestry);
    SearchContext ctx(depthLimit, searchAncestry, moveFinder);

    SpiderTableau tableau(parentTableau);
    // Put the current position in the history (prevent search loops)
    std::string parentTabString = parentTableau.GetTableauString();
    ctx.AddParentPosition(parentTabString);

    std::vector<ScoredMove> scoredMoves;
    for (auto move : firstMoves)
    {
        SpiderTableau::SavePoint save(tableau);
        tableau.DoMove(move, DoTurnCard::No);

        std::string tabString = tableau.GetTableauString();
        if (ctx.IsAParentPosition(tabString))
        {
            disregardedMoves.push_back(move);
            continue;
        }

        float score = ComputeScore(tableau);
        scoredMoves.push_back(ScoredMove(score, move));
    }
    SortScoredMoves(scoredMoves);
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
    std::vector<MoveCombo>& disregardedMoves,
    SearchContext& ctx)
{
    m_topLevelDisregardedMoves.clear();

    auto treeNodes = FindAndScoreToDepth(1, ctx, {}, parentTableau);

    disregardedMoves = m_topLevelDisregardedMoves;

    // convert to scored moves
    std::vector<ScoredMove> result;
    for (TreeNode& tn : treeNodes)
    {
        result.push_back(ScoredMove(tn.GetScore(), tn.GetMove()));
    }
    return result;
}


std::vector<TreeNode> Strategy::FindAndScoreToDepth(
    int depth,
    SearchContext& ctx,
    std::vector<MoveCombo> pathToHere,
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
    std::vector<TreeNode> treeNodes;

    pathToHere.resize(depth);

    // If we are not at the leaf level, then just expand down.
    for (auto& move : moves)
    {
        pathToHere[depth-1] = move;

        SpiderTableau::SavePoint save(tableau);
        tableau.DoMove(move, DoTurnCard::No);

        std::string tabString = tableau.GetTableauString();

        // if we have seen this before as a parent, then skip it.
        if (ctx.IsAParentPosition(tabString))
        {
            if (depth == 1)
                m_topLevelDisregardedMoves.push_back(move);
            continue;
        }

        // check if we have seen this know before in the search.
        // [Direct parents are a special case addressed above.]
        SpiderNode foundSpiderNode;
        TreeNode treeNode;

        if (ctx.TryFindSpiderNode(tabString, foundSpiderNode))
        {
            // We have seen this exact Spider Position before.
            // So the previous analysis that looked below this can be reused.
            // Unless we are at a lesser depth that the previous analysis.
            // Then we can go deeper outselves.
            if (foundSpiderNode.GetDepth() <= depth)
            {
                // If we have seen this position elsewhere in the search (but not
                // a direct parent)  then recapture the data we already know.
                treeNode = TreeNode(foundSpiderNode.GetScore(), move);
                treeNodes.push_back(treeNode);
                continue;
            }
            // We have seen this node before, but we have deep analysis.
            // Then remove the old one and continue.
            ctx.RemoveSpiderNode(tabString);
        }

        if (depth >= ctx.GetMaxDepth())
        {
            // Are we at maximum depth.  Don't go deeper.
            // Compute Score as a "leaf node"
            float score = (float)ComputeScore(tableau);
            treeNode = TreeNode(score, move);
        }
        else
        {
            // Recurse deeper.
            std::vector<TreeNode> childTreeNodes = FindAndScoreToDepth(depth + 1, ctx, pathToHere, tableau);
            if (childTreeNodes.size() > 0)
            {
                // If there are children then score is the best of its children.
                treeNode = TreeNode(childTreeNodes[0].GetScore(), move);
            }
            else
            {
                // If no children then this nodes score is its score.
                float score = (float)ComputeScore(tableau);
                treeNode = TreeNode(score, move);
            }
        }
        treeNodes.push_back(treeNode);
        ctx.AddSpiderNode(SpiderNode(depth, tabString, treeNode.GetScore()));
    }
    SortTreeNodes(treeNodes);
    ctx.RemoveParentPosition(parentTabString);
    return treeNodes;
}
