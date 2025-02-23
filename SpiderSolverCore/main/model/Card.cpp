#include "spidersolvercore/Model/Card.h"
#include "spidersolvercore/Core/SpiderException.h"

#include "assert.h"

namespace
{
    const Rank s_valueToRank[64] = {
        Rank::Ace, Rank::Two, Rank::Three, Rank::Four,
        Rank::Five, Rank::Six, Rank::Seven, Rank::Eight,
        Rank::Nine, Rank::Ten, Rank::Jack, Rank::Queen,
        Rank::King, Rank::NaR, Rank::NaR, Rank::NaR,

        Rank::Ace, Rank::Two, Rank::Three, Rank::Four,
        Rank::Five, Rank::Six, Rank::Seven, Rank::Eight,
        Rank::Nine, Rank::Ten, Rank::Jack, Rank::Queen,
        Rank::King, Rank::NaR, Rank::NaR, Rank::NaR,

        Rank::Ace, Rank::Two, Rank::Three, Rank::Four,
        Rank::Five, Rank::Six, Rank::Seven, Rank::Eight,
        Rank::Nine, Rank::Ten, Rank::Jack, Rank::Queen,
        Rank::King, Rank::NaR, Rank::NaR, Rank::NaR,

        Rank::Ace, Rank::Two, Rank::Three, Rank::Four,
        Rank::Five, Rank::Six, Rank::Seven, Rank::Eight,
        Rank::Nine, Rank::Ten, Rank::Jack, Rank::Queen,
        Rank::King, Rank::NaR, Rank::NaR, Rank::NaR,
    };

    const Suit s_valueToSuit[64] = {
        Suit::Spades, Suit::Spades, Suit::Spades, Suit::Spades,
        Suit::Spades, Suit::Spades, Suit::Spades, Suit::Spades,
        Suit::Spades, Suit::Spades, Suit::Spades, Suit::Spades,
        Suit::Spades, Suit::NaS, Suit::NaS, Suit::NaS,

        Suit::Hearts, Suit::Hearts, Suit::Hearts, Suit::Hearts,
        Suit::Hearts, Suit::Hearts, Suit::Hearts, Suit::Hearts,
        Suit::Hearts, Suit::Hearts, Suit::Hearts, Suit::Hearts,
        Suit::Hearts, Suit::NaS, Suit::NaS, Suit::NaS,

        Suit::Diamonds, Suit::Diamonds, Suit::Diamonds, Suit::Diamonds,
        Suit::Diamonds, Suit::Diamonds, Suit::Diamonds, Suit::Diamonds,
        Suit::Diamonds, Suit::Diamonds, Suit::Diamonds, Suit::Diamonds,
        Suit::Diamonds, Suit::NaS, Suit::NaS, Suit::NaS,

        Suit::Clubs, Suit::Clubs, Suit::Clubs, Suit::Clubs,
        Suit::Clubs, Suit::Clubs, Suit::Clubs, Suit::Clubs,
        Suit::Clubs, Suit::Clubs, Suit::Clubs, Suit::Clubs,
        Suit::Clubs, Suit::NaS, Suit::NaS, Suit::NaS,
    };

    const int ValueMask = 0x3F;
    const int FaceUpBit = 0x40;

    uint8_t RankAndSuitToValue(Rank rank, Suit suit)
    {
        int suitPart = static_cast<int>(suit) * 16;
        int rankPart = static_cast<int>(rank);
        return suitPart + rankPart;
    }
}


Card::Card(Rank rank, Suit suit)
{
    int suitPart = static_cast<int>(suit) * 16;
    int rankPart = static_cast<int>(rank);
    m_cardValue = suitPart + rankPart;
#ifdef _DEBUG
    SetName();
#endif
}


Card::Card(const Card& card)
    : m_cardValue(card.m_cardValue)
{
#ifdef _DEBUG
    SetName();
#endif
}


Card::Card()
    : m_cardValue(-1)
#ifdef _DEBUG
    , m_name("")
#endif
{
}


Card::~Card() = default;

void Card::AssertCardIsValid() const
{
    if (m_cardValue < 0)
        throw SpiderException("Use of an uninitialize card.");
    Rank rank = s_valueToRank[m_cardValue & ValueMask];
    Suit suit = s_valueToSuit[m_cardValue & ValueMask];
    if (rank == Rank::NaR || suit == Suit::NaS)
        throw SpiderException("Use of an bad card.");
}


void Card::AssertCardIsFaceUp() const
{
    if (!(m_cardValue & FaceUpBit))
        throw SpiderException("Use of a face down card.");
}


bool Card::operator==(const Card& rhs) const
{
#ifdef _DEBUG
    AssertCardIsValid();
    rhs.AssertCardIsValid();
#endif
    return (m_cardValue & ValueMask) == (rhs.m_cardValue & ValueMask);
}


bool Card::operator!=(const Card& rhs) const
{
    return !(*this == rhs);
}


Rank Card::getRank() const
{
#ifdef _DEBUG
    AssertCardIsValid();
    AssertCardIsFaceUp();
#endif
    return s_valueToRank[m_cardValue & ValueMask];
}


int Card::getSortValue() const
{
#ifdef _DEBUG
    AssertCardIsValid();
    AssertCardIsFaceUp();
#endif
    return m_cardValue & ValueMask;
}


Suit Card::getSuit() const
{
#ifdef _DEBUG
    AssertCardIsValid();
    AssertCardIsFaceUp();
#endif
    return s_valueToSuit[m_cardValue & ValueMask];
}


bool Card::IsFaceUp() const
{
#ifdef _DEBUG
    AssertCardIsValid();
#endif
    return m_cardValue & FaceUpBit;
}


Card& Card::TurnFaceUp()
{
#ifdef _DEBUG
    AssertCardIsValid();
#endif
    m_cardValue |= FaceUpBit;
#ifdef _DEBUG
    m_name[0] = ' ';
#endif
    return *this;
}


Card& Card::TurnFaceDown()
{
#ifdef _DEBUG
    AssertCardIsValid();
#endif
    m_cardValue &= ValueMask;
    return *this;
}

#ifdef _DEBUG
void Card::SetName()
{
    static char Suits[] = "SHDC";
    static char Ranks[] = "A23456789TJQK";
    Suit suitPart = s_valueToSuit[m_cardValue & ValueMask];
    Rank rankPart = s_valueToRank[m_cardValue & ValueMask];
    m_name[0] = IsFaceUp() ? ' ' : 'd';
    m_name[1] = Ranks[static_cast<int>(rankPart)];
    m_name[2] = Suits[static_cast<int>(suitPart)];
    m_name[3] = 0;
}
#endif
