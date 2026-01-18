#pragma once

#include "spidersolvercore/model/SpiderTableau.h"
#include "spidersolvercore/model/StackStats.h"
#include "spidersolvercore/model/SpiderConstants.h"

#include <array>
#include <vector>

class SpiderTableau;

class TableauStats
{
public:
    static const int NumDealtDownCards;
    static const int NumPacks;
    static const int SuitLength;

public:
    TableauStats(const SpiderTableau& tableau);

    int GetNumberOfHoles() const;
    int GetNumberOfTurnedCards() const;  // 44 - down cards
    int GetNumberOfCompletedPacks() const;
    int GetNumberOfDealsRemaining() const;
    const std::array<int, NUM_RANKS> GetRunLengthCounts() const;

    static int WinNumberOfHoles();
    static int WinNumberOfTurnedCards(); // 44 - down cards
    static int WinNumberOfCompletedPacks();

private:
    int ComputeNumberOfHoles();
    int ComputeNumberOfTurnedCards();
    std::array<int, NUM_RANKS> ComputeRunLengthCounts();

protected:
    int m_numberOfHoles;
    int m_numberOfTurnedCards;
    int m_numberOfCompletedPacks;
    int m_numberOfDealsRemaining;

    std::array<int, NUM_RANKS> m_runLengthCounts;
    std::array<StackStats, SpiderTableau::NUM_STACKS> m_stackStats;
};