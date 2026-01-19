#include "spidersolvercore/strategy/BoardStats.h"
#include "spidersolvercore/model/SpiderConstants.h"

BoardStats::BoardStats(const SpiderTableau& tableau)
    : TableauStats(tableau)
{
    ComputeSuitedRunScore();
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

void BoardStats::ComputeSuitedRunScore()
{
    float suitedRunLengthSum = 0;
    for (int len = 1; len < SuitLength; len++)
    {
        suitedRunLengthSum += RunLengthScore(len) * m_runLengthCounts[len];
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
    float TurnedCardsStackScore[] = { 15, 10, 6, 3, 1, 0 };
    float HoleCountScores[] = { 0, 5, 7, 9, 10, 10, 10, 10, 10, 10, 10 };
}

// -------- Turned Cards Depth Score

void BoardStats::ComputeTurnedCardDepthScore()
{
    float turnedCardsScore = 0;
    for (auto& stat : m_stackStats)
        turnedCardsScore += TurnedCardsStackScore[stat.NumberOfDownCards()];

    m_turnedCardDepthScore = turnedCardsScore;
}

float BoardStats::GetTurnedCardDepthScore() const
{
    return m_turnedCardDepthScore;
}



float BoardStats::WinTurnedCardDepthScore()
{
    return TurnedCardsStackScore[0] * SpiderTableau::NUM_STACKS;
}

// --------- Hole Score

float BoardStats::GetHoleScore() const
{
    return HoleCountScores[m_numberOfHoles];
}

float BoardStats::WinHoleScore()
{
    return HoleCountScores[10];
}
