#pragma once

#include "spidersolvercore/Model/SpiderTableau.h"
#include "spidersolvercore/Model/StackStats.h"
#include "spidersolvercore/Model/TableauStats.h"

#include <array>
#include <vector>

class SpiderTableau;

class BoardStats : public TableauStats
{
public:
    BoardStats(const SpiderTableau& tableau);

    float GetSuitedRunsScore() const;
    static float WinSuitedRunsScore();
    //const ScoreStat GetSuitedRunStats() const;

    float GetTurnedCardDepthScore() const;
    static float WinTurnedCardDepthScore();

    float GetHoleScore() const;
    static float WinHoleScore();

private:
    void ComputeSuitedRunScore();
    void ComputeTurnedCardDepthScore();

private:
    float m_suitedRunScore;
    float m_turnedCardDepthScore;
    float m_holeScore;
};
