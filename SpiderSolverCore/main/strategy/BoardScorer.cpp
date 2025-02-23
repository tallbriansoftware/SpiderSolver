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
    , m_turnedCardsTerm(3)
    , m_fullPackTerm(6)
{
}

float BoardScorer::ComputeScore(const SpiderTableau& tableau) const
{
    BoardStats tscore(tableau);

    float total = 0.0;

    total += m_suitedRunTerm * tscore.GetSuitedRunsScore();
    total += m_holesTerm * tscore.GetHoleScore();
    total += m_turnedCardsTerm * tscore.GetTurnedCardDepthScore();
    total += m_fullPackTerm * tscore.GetNumberOfCompletedPacks();

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

void BoardScorer::SetHolesTerm(float term)
{
    m_setTerms |= HolesTerm;
    m_holesTerm = term;
}

void BoardScorer::SetTurnedCardsTerm(float term)
{
    m_setTerms |= TurnedCardsTerm;
    m_turnedCardsTerm = term;
}


std::vector<std::string> BoardScorer::GetModifiedTermNames() const
{
    std::vector<std::string> list;
    
    if (m_setTerms & HolesTerm)
        list.push_back("HolesTerm");
    if (m_setTerms & TurnedCardsTerm)
        list.push_back("TurnedCardsTerm");
    if (m_setTerms & FullPackTerm)
        list.push_back("FullPackTerm");

    return list;
}

std::vector<float> BoardScorer::GetModifiedTerms() const
{
    std::vector<float> list;

    if (m_setTerms & HolesTerm)
        list.push_back(m_holesTerm);
    if (m_setTerms & TurnedCardsTerm)
        list.push_back(m_turnedCardsTerm);
    if (m_setTerms & FullPackTerm)
        list.push_back(m_fullPackTerm);

    return list;
}