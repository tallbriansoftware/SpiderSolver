#include "spidersolvercore/strategy/ScoredMove.h"

#include "spidersolvercore/strategy/Strategy.h"
#include "spidersolvercore/strategy/BoardScorer.h"
#include "spidersolvercore/model/SpiderTableau.h"


ScoredMove::ScoredMove(float score, const MoveCombo& move)
    : m_score(score)
    , m_move(move)
    , m_localScore(-1.0)
{ }

float ScoredMove::GetScore() const
{
    return m_score;
}

float ScoredMove::GetLocalScore() const
{
    return m_localScore;
}

const MoveCombo& ScoredMove::GetMove() const
{
    return m_move;
}

void ScoredMove::SetLocalScore(
    const Strategy& strategy,
    const SpiderTableau& parentTableau)
{
    SpiderTableau tableau(parentTableau);
    SpiderTableau::SavePoint save(tableau);
    tableau.DoMove(m_move, DoTurnCard::No);

    m_localScore = strategy.ComputeScore(tableau);
}
