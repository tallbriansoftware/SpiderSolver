#include "spidersolvercore/strategy/MoveChooser.h"

#include "spidersolvercore/logic/MoveFinder.h"
#include "spidersolvercore/strategy/ScoredMove.h"
#include "spidersolvercore/model/SpiderTableau.h"
#include "spidersolvercore/strategy/Strategy.h"
#include "spidersolvercore/strategy/StrategyUtil.h"

MoveChooser::MoveChooser(
    std::shared_ptr<SpiderTableau> tableau,
    Strategy& strategy,
    int depth)
    : m_tableau(tableau)
    , m_strategy(strategy)
    , m_ancestry(*tableau)
    , m_depth(depth)
{
}

ScoredMove MoveChooser::GetBestMove()
{
    return m_moveChoices[0];
}

bool MoveChooser::ComputeBestMove(MoveFinderFunc moveFinderFunc)
{
    m_moveChoices = m_strategy.FindScoredMoves(
        moveFinderFunc, m_disregardedChoices, *m_tableau, m_ancestry, m_depth);

    if (m_moveChoices.empty())
        return false;

    StrategyUtil::SortTiedBestMoves(m_moveChoices, m_strategy, *m_tableau);
    return true;
}


MoveCombo MoveChooser::ComputeBestMove()
{
    float boardScore = m_strategy.ComputeScore(*m_tableau);

    // In normal cases look for moves that:
    // - Don't consume holes
    // - Don't split suited runs.
    if (ComputeBestMove(MoveFindingFunc::Normal))
    {
        // If we know a path that will improve the score
        // then take it.
        if (GetBestMove().GetScore() > boardScore)
            return GetBestMove().GetMove();
    }

    // If there are not moves that lead to an improvment
    // and we don't have any holes then "Deal".
    if (m_tableau->GetHoleCount() == 0)
        return MoveCombo::None();

    // If there are holes then look for *any* move that
    // that might turn a card.
    if (ComputeBestMove(MoveFindingFunc::Any))
        return GetBestMove().GetMove();

    //if(ComputeBestMove())
    return MoveCombo::None();

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

int MoveChooser::GetMoveNumber() const
{
    return m_ancestry.GetLength();
}
