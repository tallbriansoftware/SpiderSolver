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

MoveCombo MoveChooser::ComputeBestMove()
{
    auto moveFinderFunc = MoveFinder::NormalAndHoleFilling;

    m_moveChoices = m_strategy.FindScoredMoves(
        moveFinderFunc, m_disregardedChoices, *m_tableau, m_ancestry, m_depth);

    if(m_moveChoices.empty())
    {
        if (m_tableau->CanDeal())
            return MoveCombo::Deal();
        return MoveCombo::None();
    }

    StrategyUtil::SortTiedBestMoves(m_moveChoices, m_strategy, *m_tableau);
    return m_moveChoices[0].GetMove();
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

const std::vector<ScoredMove>& MoveChooser::GetAllChoices() const
{
    return m_moveChoices;
}


const std::vector<MoveCombo>& MoveChooser::GetDisregardedChoices() const
{
    return m_disregardedChoices;
}

int MoveChooser::GetMoveNumber() const
{
    return m_ancestry.GetLength();
}
