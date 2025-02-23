#pragma once

#include "spidersolvercore/Model/CardEnums.h"

class Card
{
public:
    Card(Rank rank, Suit suit);
    Card(const Card& card);
    Card();

    Rank getRank() const;
    Suit getSuit() const;

    int getSortValue() const;

    bool IsFaceUp() const;
    Card& TurnFaceUp();
    Card& TurnFaceDown();

    ~Card();
    bool operator==(const Card& rhs) const;
    bool operator!=(const Card& rhs) const;

private:
    void AssertCardIsValid() const;
    void AssertCardIsFaceUp() const;

#ifdef _DEBUG
    void SetName();
#endif

private:
    char m_cardValue;
#ifdef _DEBUG
public:
    char m_name[4];
#endif
};
