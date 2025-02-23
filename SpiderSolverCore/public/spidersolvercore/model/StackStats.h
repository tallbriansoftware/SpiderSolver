#pragma once

#include <vector>

class SpiderStack;

class StackStats
{
public:
    StackStats();
    StackStats(const SpiderStack& stack);
    void Init(const SpiderStack& stack);

    int NumberOfCards() const;
    int NumberOfDownCards() const;
    int NumberOfUpCards() const;

    const std::vector<int>& SuitedRunLengths() const;
    int NumberOfRunBreaks() const;
    int NumberOfRankBreaks() const;

    bool HasFullPack() const;

private:
    int m_numberOfDownCards;
    int m_numberOfUpCards;
    int m_numberOfCards;

    std::vector<int> m_suitedRunLengths;
    int m_numberOfRankBreaks;

};