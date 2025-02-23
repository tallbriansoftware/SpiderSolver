#pragma once

#include "spidersolvercore/Model/SpiderTableau.h"
#include "spidersolvercore/Model/StackStats.h"

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

    static int WinNumberOfHoles();
    static int WinNumberOfTurnedCards(); // 44 - down cards
    static int WinNumberOfCompletedPacks();

private:
    int ComputeNumberOfHoles();
    int ComputeNumberOfTurnedCards();

protected:
    int m_numberOfHoles;
    int m_numberOfTurnedCards;
    int m_numberOfCompletedPacks;
    int m_numberOfDealsRemaining;

    std::array<StackStats, SpiderTableau::NUM_STACKS> m_stackStats;
};