#include "spidersolvercore/Model/StackStats.h"

#include "spidersolvercore/Model/SpiderStack.h"

#include <assert.h>


StackStats::StackStats()
    : m_numberOfCards(0)
    , m_numberOfDownCards(0)
    , m_numberOfUpCards(0)
    , m_numberOfRankBreaks(0)
{
}


StackStats::StackStats(const SpiderStack& stack)
{
    Init(stack);
}


void StackStats::Init(const SpiderStack& stack)
{
    m_numberOfCards = stack.Count();
    m_numberOfDownCards = stack.CountDownCards();
    m_numberOfUpCards = stack.CountUpCards();

    int numRuns = stack.GetRunCount();
    m_numberOfRankBreaks = 0;
    if (numRuns == 0 && m_numberOfDownCards != 0)
    {
        if (m_numberOfDownCards != 0)
            m_suitedRunLengths.push_back(1);
    }
    else if (numRuns > 0)
    {
        int head = stack.GetRunHead(0);
        int tail = stack.GetRunTail(0);
        m_suitedRunLengths.push_back(tail - head + 1);

        // Do the rest and assume there is a previous run.
        if (numRuns > 1)
        {
            for (int run = 1; run < numRuns; ++run)
            {
                int head = stack.GetRunHead(run);
                int tail = stack.GetRunTail(run);
                m_suitedRunLengths.push_back(tail - head + 1);

                const Card& previousHeadCard = stack.GetCard(tail + 1);
                const Card& tailCard = stack.GetCard(tail);
                if ((int)tailCard.getRank() != (int)previousHeadCard.getRank() + 1)
                    m_numberOfRankBreaks += 1;
            }
        }
    }
}

int StackStats::NumberOfCards() const
{
    return m_numberOfCards;
}

int StackStats::NumberOfDownCards() const
{
    return m_numberOfDownCards;
}

int StackStats::NumberOfUpCards() const
{
    return m_numberOfUpCards;
}

int StackStats::NumberOfRunBreaks() const
{
    int num = (int)SuitedRunLengths().size();
    if (num > 0)
        return num - 1;
    return 0;
}

int StackStats::NumberOfRankBreaks() const
{
    return m_numberOfRankBreaks;
}

const std::vector<int>& StackStats::SuitedRunLengths() const
{
    return m_suitedRunLengths;
}

