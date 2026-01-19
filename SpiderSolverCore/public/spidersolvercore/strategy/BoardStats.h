#pragma once

#include "spidersolvercore/Model/SpiderTableau.h"
#include "spidersolvercore/Model/StackStats.h"
#include "spidersolvercore/Model/TableauStats.h"

#include <array>
#include <vector>
#include <memory>

class SpiderTableau;

using ScoreStat = std::pair<float, std::vector<int>>;

class BoardStats : public TableauStats
{
public:
    BoardStats(const SpiderTableau& tableau);

    float GetHoleScore() const;
    ScoreStat GetHoleStats() const;
    static float WinHoleScore();

    float GetDownCardScore() const;
    ScoreStat GetDownCardStats() const;
    static float WinDownCardScore();

    float GetSuitedRunsScore() const;
    ScoreStat GetSuitedRunStats() const;
    static float WinSuitedRunsScore();

private:
    void ComputeSuitedRunScore();
    void ComputeTurnedCardDepthScore();

private:
    float m_suitedRunScore;
    float m_downCardScore;
    float m_holeScore;

    mutable std::unique_ptr<ScoreStat> m_holesStats;
    mutable std::unique_ptr<ScoreStat> m_downCardStats;
    mutable std::unique_ptr<ScoreStat> m_suitedRunsStats;
};
