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

    float DownCardsStackScore[] = { 15, 10, 6, 3, 1, 0 };
    float HoleCountScores[] = { 0, 20, 30, 40, 50, 50, 50, 50, 50, 50, 50 };
}

// --------- Hole Score

float BoardStats::GetHoleScore() const
{
    return HoleCountScores[m_numberOfHoles];
}

ScoreStat BoardStats::GetHoleStats() const
{
    if (m_holesStats == nullptr)
    {
        std::vector<int> holes;
        holes.push_back(m_numberOfHoles);
        m_holesStats = std::make_unique<ScoreStat>(GetHoleScore(), holes);
    }
    return *m_holesStats;
}

float BoardStats::WinHoleScore()
{
    return HoleCountScores[10];
}

// --------- Suited Runs

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

ScoreStat BoardStats::GetSuitedRunStats() const
{
    if (m_suitedRunsStats == nullptr)
    {
        std::vector<int> runs(std::begin(m_runLengthCounts), std::end(m_runLengthCounts));
        runs.push_back(m_numberOfCompletedPacks);
        m_suitedRunsStats = std::make_unique<ScoreStat>(GetSuitedRunsScore(), runs);
    }
    return *m_suitedRunsStats;
}

float BoardStats::WinSuitedRunsScore()
{
    return NumPacks * RunLengthScore(SuitLength);
}

// -------- Turned Cards Depth Score

void BoardStats::ComputeTurnedCardDepthScore()
{
    float turnedCardsScore = 0;
    for (auto& stat : m_stackStats)
        turnedCardsScore += DownCardsStackScore[stat.NumberOfDownCards()];

    m_downCardScore = turnedCardsScore;
}

float BoardStats::GetDownCardScore() const
{
    return m_downCardScore;
}

ScoreStat BoardStats::GetDownCardStats() const
{
    if (m_suitedRunsStats == nullptr)
    {
        std::vector<int> numDown(6);
        for (auto& stat : m_stackStats)
            numDown[stat.NumberOfDownCards()] += 1;
        m_downCardStats = std::make_unique<ScoreStat>(GetDownCardScore(), numDown);
    }
    return *m_downCardStats;
}


float BoardStats::WinDownCardScore()
{
    return DownCardsStackScore[0] * SpiderTableau::NUM_STACKS;
}
