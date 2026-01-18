#include "spidersolvercore/strategy/BoardScorer.h"

#include "spidersolvercore/Model/SpiderTableau.h"
#include "spidersolvercore/strategy/BoardStats.h"

const int HolesTerm = 0x01;
const int TurnedCardsTerm = 0x02;
const int FullPackTerm = 0x04;

BoardScorer::BoardScorer()
    : m_setTerms(0)
    , m_suitedRunTerm(1.0)  // Everything is relative to this.
    , m_holesTerm(10)
    , m_turnedCardsTerm(1)
    , m_fullPackTerm(10)
{
}

float BoardScorer::ComputeScore(const SpiderTableau& tableau) const
{
    BoardStats bscore(tableau);

    float total = 0.0;

    total += m_suitedRunTerm * bscore.GetSuitedRunsScore();
    total += m_holesTerm * bscore.GetHoleScore();
    total += m_turnedCardsTerm * bscore.GetTurnedCardDepthScore();
    total += m_fullPackTerm * bscore.GetNumberOfCompletedPacks();

    return total;
}

float BoardScorer::MaxScore() const
{
    float total = 0.0;

    total += m_suitedRunTerm * BoardStats::WinSuitedRunsScore();
    total += m_holesTerm * BoardStats::WinHoleScore();
    total += m_turnedCardsTerm * BoardStats::WinTurnedCardDepthScore();
    total += m_fullPackTerm * BoardStats::WinNumberOfCompletedPacks();

    return total;
}
