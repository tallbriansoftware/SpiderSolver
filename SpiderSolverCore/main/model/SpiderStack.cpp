#include "spidersolvercore/Model/SpiderStack.h"
#include "spidersolvercore/Core/SpiderException.h"
#include "spidersolvercore/Model/SpiderString.h"

#include <assert.h>


SpiderStack::SpiderStack()
    : m_isColumn(false)
{ }


SpiderStack::SpiderStack(const std::vector<Card>& cards)
    : m_isColumn(false)
    , m_cards(cards)
{ }


SpiderStack::SpiderStack(const SpiderStack& stack)
    : m_isColumn(stack.m_isColumn)
    , m_cards(stack.m_cards)
    , m_heads(stack.m_heads)
    , m_string(stack.m_string)
{ }


SpiderStack& SpiderStack::operator=(const SpiderStack& stack)
{
    m_isColumn = stack.m_isColumn;
    m_cards = stack.m_cards;
    m_heads = stack.m_heads;
    m_string = stack.m_string;
    return *this;
}

SpiderStack::~SpiderStack() = default;


void SpiderStack::SetIsColumn()
{
    m_isColumn = true;
    m_heads.clear();
    m_string.clear();
}

int SpiderStack::Count() const
{
    return (int)m_cards.size();
}


bool SpiderStack::IsEmpty() const
{
    return 0 == (int)m_cards.size();
}


bool SpiderStack::IsEmptyOrTopCardNotFaceUp() const
{
    return IsEmpty() || !PeekTopCard().IsFaceUp();
}


void SpiderStack::AddNewCard(const Card& card)
{
    m_cards.emplace_back(Card(card));
    m_heads.clear();
    m_string.clear();
}


void SpiderStack::MoveCard(SpiderStack& srcStack)
{
    int count = srcStack.Count();
    m_cards.push_back(std::move(srcStack.m_cards.back()));

    srcStack.Resize(count - 1);
    m_heads.clear();
    m_string.clear();
}


void SpiderStack::MoveCards(SpiderStack& srcStack, int srcIndex)
{
    auto fromStart = srcStack.m_cards.begin() + srcIndex;
    auto fromEnd = srcStack.m_cards.end();

    for (auto& itr = fromStart; itr != fromEnd; ++itr)
        m_cards.push_back(*itr);

    srcStack.Resize(srcIndex);
    m_heads.clear();
    m_string.clear();
}


const Card& SpiderStack::PeekTopCard() const
{
    return m_cards.back();
}


bool SpiderStack::WouldTurnUpTopCard() const
{
    if (m_cards.size() > 0 && !m_cards.back().IsFaceUp())
        return true;
    return false;
}


bool SpiderStack::TurnUpTopCard()
{
    bool ret = WouldTurnUpTopCard();
    if (ret)
    {
        m_cards.back().TurnFaceUp();
        m_heads.clear();
        m_string.clear();
    }
    return ret;
}


bool SpiderStack::TurnDownTopCard()
{
    // Top card can be face down when seaching for a move.
    if (m_cards.size() > 0)
    {
        // Fail if there is more than one up card.
        if (m_cards.size() > 1 && m_cards[m_cards.size() - 2].IsFaceUp())
            return false;
        m_cards.back().TurnFaceDown();
        m_heads.clear();
        m_string.clear();
        return true;
    }
    return false;
}


const std::vector<Card>& SpiderStack::GetCards() const
{
    return m_cards;
}


// for debugging
std::vector<Card>& SpiderStack::GetMutableCards()
{
    return m_cards;
}


const Card& SpiderStack::GetCard(int index) const
{
    if (index < 0)
        index = (int)m_cards.size() + index;
    return m_cards[index];
}


int SpiderStack::IndexOf(const Card& card) const
{
    return (int)(&card - &m_cards[0]);
}


int SpiderStack::GetHeadMostCardIndex() const
{
    if (IsEmpty())
        return -1;

    // Sometimes the top card is face down.
    auto& heads = GetRunHeads();
    if (heads.empty())
        return -1;

    return heads[heads.size() - 1];
}


int SpiderStack::GetRunCount() const
{
    if (!m_isColumn)
        throw SpiderException("Spider Stack must be 'Column'");

    auto& heads = GetRunHeads();
    return (int)heads.size();
}


int SpiderStack::GetRunHead(int deep) const
{
    if (!m_isColumn)
        throw SpiderException("Spider Stack must be 'Column'");

    assert(deep >= 0);

    auto& heads = GetRunHeads();

    if (deep >= (int)heads.size())
        return -1;
    return heads[deep];
}


int SpiderStack::GetRunTail(int deep) const
{
    if (!m_isColumn)
        throw SpiderException("Spider Stack must be 'Column'");

    auto& heads = GetRunHeads();

    assert(deep >= 0);

    if (deep >= (int)m_heads.size())
        return -1;

    if (deep == 0)
        return (int)m_cards.size() - 1;

    // return the card before the head of the run before.
    return m_heads[deep - 1] - 1;
}


int SpiderStack::CountDownCards() const
{
    if (IsEmpty())
        return 0;

    // sometimes the top card is face down,
    // for the purpose of score etc we realize that
    // it will soon be turned face up
    // so it doesn't count as down.
    int index = GetHeadMostCardIndex();
    if (index < 0)
        return Count() - 1;
    return index;
}


int SpiderStack::CountUpCards() const
{
    if (IsEmpty())
        return 0;

    int index = GetHeadMostCardIndex();
    if (index == -1)
        return 0;

    return Count() - index;
}


bool SpiderStack::CanRemoveFullPack() const
{
    int index = GetRunHead(0);
    return (index == (Count() - 13));
}


bool SpiderStack::operator!=(const SpiderStack& rhs) const
{
    return !(*this == rhs);
}


bool SpiderStack::operator==(const SpiderStack& rhs) const
{
    if (m_cards.size() != rhs.m_cards.size())
        return false;
    for (int i = 0; i < (int)m_cards.size(); ++i)
    {
        if (m_cards[i] != rhs.m_cards[i])
            return false;

        if (m_cards[i].IsFaceUp() != rhs.m_cards[i].IsFaceUp())
            return false;
    }
    return (m_isColumn == rhs.m_isColumn);
}


const std::string& SpiderStack::GetStackString() const
{
    if (m_string.size() == 0)
        m_string = SpiderString::Create(*this);
    return m_string;
}


void SpiderStack::Resize(int cardIndex)
{
    m_cards.resize(cardIndex);
    m_heads.clear();
    m_string.clear();
}


std::vector<int>& SpiderStack::GetRunHeads() const
{
    // return the existing non-zero cached list.
    if (m_heads.size() > 0)
        return m_heads;

    // return the existing empty list;
    if (IsEmpty())
        return m_heads;

    // Sometimes the top card is not turned.
    // return the existing empty list;
    const Card* previous = &PeekTopCard();
    if (!previous->IsFaceUp())
        return m_heads;

    if (Count() == 1)
    {
        m_heads.push_back(0);
        return m_heads;
    }

    // start the loop one up from the bottom.
    // having already loaded "previous" above.
    for (auto itr = m_cards.rbegin() + 1; itr != m_cards.rend(); ++itr)
    {
        auto& card = *itr;
        if (!card.IsFaceUp())
            break;
        if (card.getSortValue() != previous->getSortValue() + 1)
            m_heads.push_back(IndexOf(*previous));
        previous = &card;
    }
    m_heads.push_back(IndexOf(*previous));
    return m_heads;
}

