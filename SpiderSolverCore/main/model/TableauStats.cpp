#include "spidersolvercore/Model/TableauStats.h"

#include "spidersolvercore/Model/SpiderTableau.h"
#include "spidersolvercore/Model/StackStats.h"

#include <assert.h>


const int TableauStats::NumDealtDownCards = 44;
const int TableauStats::NumPacks = 8;
const int TableauStats::SuitLength = 13;


TableauStats::TableauStats(const SpiderTableau& tableau)
    : m_numberOfCompletedPacks(tableau.GetPacksRemoved())
    , m_numberOfDealsRemaining(tableau.GetDealsRemaining())
{
    for (int i = 0; i < SpiderTableau::NUM_STACKS; ++i)
    {
        m_stackStats[i].Init(tableau.GetStack(i));
    }

    m_numberOfHoles = ComputeNumberOfHoles();
    m_numberOfTurnedCards = ComputeNumberOfTurnedCards();
}

int TableauStats::GetNumberOfCompletedPacks() const
{
    return m_numberOfCompletedPacks;
}

int TableauStats::WinNumberOfCompletedPacks()
{
    return NumPacks;
}

int TableauStats::GetNumberOfDealsRemaining() const
{
    return m_numberOfDealsRemaining;
}


// ---- Holes

int TableauStats::ComputeNumberOfHoles()
{
    int numberOfHoles = 0;
    for (auto& stat : m_stackStats)
        if (stat.NumberOfCards() == 0)
            numberOfHoles += 1;
    return numberOfHoles;
}

int TableauStats::GetNumberOfHoles() const
{
    return m_numberOfHoles;
}

int TableauStats::WinNumberOfHoles()
{
    return SpiderTableau::NUM_STACKS;
}

// ------ Turned Cards

int TableauStats::ComputeNumberOfTurnedCards()
{
    int downCards = 0;
    for (auto& stat : m_stackStats)
        downCards += stat.NumberOfDownCards();

    return NumDealtDownCards - downCards;
}

int TableauStats::GetNumberOfTurnedCards() const
{
    return m_numberOfTurnedCards;
}

int TableauStats::WinNumberOfTurnedCards()
{
    return NumDealtDownCards;
}
