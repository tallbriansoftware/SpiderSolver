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

    int GetTurnedCardDepthScore() const;
    static int WinTurnedCardDepthScore();

    int GetHoleScore() const;
    static int WinHoleScore();

private:
    void ComputeNumberOfSuitedRunCards();
    void ComputeTurnedCardDepthScore();

private:
    float m_suitedRunScore;
    int m_turnedCardDepthScore;
    int m_holeScore;
};