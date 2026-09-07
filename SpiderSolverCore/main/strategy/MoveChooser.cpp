#include "spidersolvercore/strategy/MoveChooser.h"

#include "spidersolvercore/logic/MoveFinder.h"
#include "spidersolvercore/strategy/ScoredMove.h"
#include "spidersolvercore/model/SpiderTableau.h"
#include "spidersolvercore/strategy/Strategy.h"
#include "spidersolvercore/strategy/StrategyUtil.h"

#include <algorithm>

namespace
{
    void SortScoredMoves(std::vector<ScoredMove>& scoredMoves)
    {
        // Sort the moves by score,  greatest to least.
        std::sort(begin(scoredMoves), end(scoredMoves), [](ScoredMove& a, ScoredMove& b)
            {
                return a.GetScore() > b.GetScore();
            });
    }

    MoveCombo NoneOrDeal(const SpiderTableau& tableau)
    {
        if (tableau.CanDeal())
            return MoveCombo::Deal();
        return MoveCombo::None();
    }

    bool IsAParentPosition(const Ancestry& ancestry, std::string tabString)
    {
        return (ancestry.FindRepeatIndex(tabString) > 0);
    }
}

MoveChooser::MoveChooser(
    std::shared_ptr<SpiderTableau> tableau,
    Strategy& strategy,
    int maxDepth)
    : m_tableau(tableau)
    , m_strategy(strategy)
    , m_ancestry(*tableau)
    , m_maxDepth(maxDepth)
{
}

ScoredMove MoveChooser::GetBestMove()
{
    return m_moveChoices[0];
}

bool MoveChooser::ComputeBestMove(
                MoveFinderFunc moveFinderFunc,
                const SpiderTableau& tableau)
{
    m_moveChoices = m_strategy.FindScoredMoves(
        moveFinderFunc, m_disregardedChoices, tableau, m_ancestry, m_maxDepth);

    if (m_moveChoices.empty())
        return false;

    StrategyUtil::SortTiedBestMoves(m_moveChoices, m_strategy, tableau);
    return true;
}

bool MoveChooser::ComputeBestMoveThatFillsAHole()
{
    auto holeMoves = MoveFinder::JustHoleFilling(*m_tableau);
    if (holeMoves.size() == 0)
        return false;

    std::vector<ScoredMove> resultMoves;
    SpiderTableau tableau(*m_tableau);
    for (auto currentHoleMove : holeMoves)
    {
        SpiderTableau::SavePoint save(tableau);
        tableau.DoMove(currentHoleMove, DoTurnCard::No);

        std::string tabString = tableau.GetTableauString();
        if (IsAParentPosition(m_ancestry, tabString))
            continue;

        float score = m_strategy.ComputeScore(tableau);

        if (ComputeBestMove(MoveFinder::Any, tableau))
        {
            score = GetBestMove().GetScore();
        }
        ScoredMove scMove(score, currentHoleMove);
        resultMoves.push_back(scMove);
    }
    m_moveChoices = resultMoves;
    m_disregardedChoices.clear();
    if (m_moveChoices.empty())
        return false;

    SortScoredMoves(m_moveChoices);
    StrategyUtil::SortTiedBestMoves(m_moveChoices, m_strategy, *m_tableau);
    return true;
}

MoveCombo MoveChooser::ComputeBestMove()
{
    float boardScore = m_strategy.ComputeScore(*m_tableau);

    // In normal cases look for moves that:
    // - Don't consume holes
    // - Only split suited runs to make longer suited runs.
    if (ComputeBestMove(MoveFinder::Normal, *m_tableau))
    {
        // Only take moves if they improve the position.
        if (GetBestMove().GetScore() > boardScore)
            return GetBestMove().GetMove();
    }

    // If there are no moves that lead to an improvment
    // and we don't have any holes then "Deal".
    if (m_tableau->GetHoleCount() == 0)
        return NoneOrDeal(*m_tableau);

    // Look for an improving move using the "Any" finder.
    // this will fill holes or split suited runs (and normal) moves.
    if (ComputeBestMove(MoveFinder::Any, *m_tableau))
    {
        // Only take moves if they improve the position.
        if (GetBestMove().GetScore() > boardScore)
            return GetBestMove().GetMove();
    }

    // If nothing can be found to improve the position.
    // Then just plug a hole before the impending "Deal"
    if (ComputeBestMoveThatFillsAHole())
        return GetBestMove().GetMove();

    return NoneOrDeal(*m_tableau);
}

void MoveChooser::CommitMove(const MoveCombo& move)
{
    if (move.IsDeal())
    {
        if (!m_tableau->DealTurn())
            throw std::exception("can't deal as expected");
    }
    else
    {
        m_tableau->DoMove(move, DoTurnCard::Auto);
    }
    m_ancestry.AddTableau(*m_tableau);
}

const std::vector<ScoredMove> MoveChooser::GetAllChoices() const
{
    return m_moveChoices;
}

const std::vector<MoveCombo> MoveChooser::GetDisregardedChoices() const
{
    return m_disregardedChoices;
}

const Strategy& MoveChooser::GetStrategy() const
{
    return m_strategy;
}


int MoveChooser::GetMoveNumber() const
{
    return m_ancestry.GetLength();
}
