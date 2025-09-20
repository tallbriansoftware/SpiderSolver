#include "spidersolvercore/strategy/BoardStats.h"

BoardStats::BoardStats(const SpiderTableau& tableau)
    : TableauStats(tableau)
{
    ComputeNumberOfSuitedRunCards();
    ComputeTurnedCardDepthScore();
}

// ------ Suited Runs

namespace
{
    float RunLengthScore(int runLength)
    {
        float fres = (float)(runLength - 1);
        float result = fres + (fres * fres / 10);
        return result;
    }
}

void BoardStats::ComputeNumberOfSuitedRunCards()
{
    float suitedRunLengthSum = 0;
    for (auto& stat : m_stackStats)
    {
        for (int runLength : stat.SuitedRunLengths())
        {
            suitedRunLengthSum += RunLengthScore(runLength);
        }
    }
    suitedRunLengthSum += m_numberOfCompletedPacks * RunLengthScore(SuitLength);

    m_suitedRunScore = suitedRunLengthSum;
}

float BoardStats::GetSuitedRunsScore() const
{
    return m_suitedRunScore;
}

float BoardStats::WinSuitedRunsScore()
{
    return NumPacks * RunLengthScore(SuitLength);
}

namespace
{
    int TurnedCardsStackScore[] = { 15, 10, 6, 3, 1, 0 };
    int HoleCountScores[] = { 0, 5, 7, 9, 10, 10, 10, 10, 10, 10, 10 };
}

// -------- Turned Cards Depth Score

void BoardStats::ComputeTurnedCardDepthScore()
{
    int turnedCardsScore = 0;
    for (auto& stat : m_stackStats)
        turnedCardsScore += TurnedCardsStackScore[stat.NumberOfDownCards()];

    m_turnedCardDepthScore = turnedCardsScore;
}

int BoardStats::GetTurnedCardDepthScore() const
{
    return m_turnedCardDepthScore;
}

int BoardStats::WinTurnedCardDepthScore()
{
    return TurnedCardsStackScore[0] * SpiderTableau::NUM_STACKS;
}

// --------- Hole Score

int BoardStats::GetHoleScore() const
{
    return HoleCountScores[m_numberOfHoles];
}

int BoardStats::WinHoleScore()
{
    return HoleCountScores[10];
}
