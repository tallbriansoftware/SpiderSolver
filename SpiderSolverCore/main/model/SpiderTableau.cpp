#include "spidersolvercore/Model/SpiderTableau.h"
#include "spidersolvercore/Model/SpiderStack.h"

#include "spidersolvercore/Model/Card.h"
#include "spidersolvercore/Model/MoveSingle.h"
#include "spidersolvercore/Model/MoveCombo.h"
#include "spidersolvercore/Model/MoveReversible.h"
#include "spidersolvercore/Core/SpiderException.h"
#include "spidersolvercore/Logic/IsMoveLegal.h"

#include <assert.h>
#include <stdlib.h>
#include <vector>
#include <memory>


SpiderTableau::SpiderTableau()
{
    for (auto& stack : m_stacks)
        stack.SetIsColumn();
}

SpiderTableau::SpiderTableau(const SpiderStack& deck)
{
    for (auto& stack : m_stacks)
        stack.SetIsColumn();
    auto myDeck(deck);
    m_stock.MoveCards(myDeck, 0);
    DealStartPosition();
}


SpiderTableau::SpiderTableau(const SpiderTableau& tableau)
    : m_stock(tableau.m_stock)
    , m_completedPacks(tableau.m_completedPacks)
    , m_stacks(tableau.m_stacks)
    // Do not copy the move stack.
{ }


SpiderTableau& SpiderTableau::operator=(const SpiderTableau& rhs)
{
    m_string.clear();
    m_stock = rhs.m_stock;
    m_completedPacks = rhs.m_completedPacks;
    m_stacks = rhs.m_stacks;
    // Do not copy the move stack.
    return *this;
}


SpiderTableau::~SpiderTableau() = default;


void SpiderTableau::InitWithDeck(SpiderStack& deck)
{
    if (deck.Count() != 104)
        throw SpiderException("Not playing with a full deck!");

    m_string.clear();
    m_stock.MoveCards(deck, 0);
    DealStartPosition();
}


void SpiderTableau::DealStartPosition()
{
    m_string.clear();
    for (int rounds = 0; rounds < 4; ++rounds)
    {
        // 4 rows of face down cards.
        for (auto& stack : m_stacks)
            stack.MoveCard(m_stock);
    }

    for (int stackNo = 0; stackNo < 4; ++stackNo)
        m_stacks[stackNo].MoveCard(m_stock);

    // deal one face up card on each stack
    for (auto& stack : m_stacks)
    {
        stack.MoveCard(m_stock);
        stack.TurnUpTopCard();
    }
}


// If DoMove is run with DoTurnCard::No
// There needs to be a way later to turn the card up.
void SpiderTableau::EnsureTopTurnedUp()
{
    for (auto& stack : m_stacks)
        stack.TurnUpTopCard();
}


// deal one face up card on each stack
bool SpiderTableau::DealTurn()
{
    if (m_stock.IsEmpty())
        return false;

    m_string.clear();
    for (auto& stack : m_stacks)
    {
        stack.MoveCard(m_stock);
        stack.TurnUpTopCard();
    }
    for (auto& stack : m_stacks)
    {
        if (stack.CanRemoveFullPack())
        {
            RemoveFullPack(stack);
            stack.TurnUpTopCard();

        }
    }
    return true;
}


void SpiderTableau::DoMove(const MoveCombo& move, DoTurnCard turnCard)
{
    for (const auto& smove : move.GetMoves())
        DoMove(smove, turnCard);
}


void SpiderTableau::DoMove(const MoveSingle& move, DoTurnCard turnCard)
{
    m_string.clear();
    SpiderStack& fromStack = m_stacks[move.FromStack()];
    SpiderStack& destStack = m_stacks[move.DestStack()];
    bool turnedCard = false;
    bool removedPack = false;
    bool packTurnedCard = false;

#ifdef _DEBUG
    if (!IsMoveLegal(*this, move))
        throw std::exception("Move is not legal");
#endif

    MoveReversible reversible = MoveReversible(move, *this);
    m_moveStack.push_back(reversible);

    destStack.MoveCards(fromStack, move.FromIndex());

    if (destStack.CanRemoveFullPack())
        RemoveFullPack(destStack);

    if (turnCard == DoTurnCard::Auto)
    {
        fromStack.TurnUpTopCard();
        // sometimes when full packs are removed.
        destStack.TurnUpTopCard();
    }
}


void SpiderTableau::UndoMove(const MoveReversible& move)
{
    m_string.clear();
    SpiderStack& fromStack = m_stacks[move.FromStack()];
    SpiderStack& destStack = m_stacks[move.DestStack()];

    if (move.TurnedCard())
    {
        if (!fromStack.TurnDownTopCard())
            throw std::exception("Undone move was not legal");
    }
    if (move.PackTurnedCard())
    {
        bool wasLegal = destStack.TurnDownTopCard();
        assert(wasLegal);
    }
    if (move.RemovedPack())
    {
        ReturnFullPack(destStack);
    }
    fromStack.MoveCards(destStack, move.DestIndex());
}


const Card& SpiderTableau::PeekTopCardAt(int stackIndex) const
{
    return m_stacks[stackIndex].PeekTopCard();
}


const Card& SpiderTableau::PeekCardAt(int stackIndex, int cardIndex) const
{
    return m_stacks[stackIndex].GetCard(cardIndex);
}


int SpiderTableau::GetDealsRemaining() const
{
    assert(m_stock.Count() % 10 == 0);
    return m_stock.Count() / 10;
}

bool SpiderTableau::CanDeal() const
{
    return !m_stock.IsEmpty();
}

int SpiderTableau::GetPacksRemoved() const
{
    assert(m_completedPacks.Count() % 13 == 0);
    return m_completedPacks.Count() / 13;
}

int SpiderTableau::GetHoleCount() const
{
    int count = 0;
    for (const auto& stack : m_stacks)
        count += stack.IsEmpty() ? 1 : 0;
    return count;
}


int SpiderTableau::FindFirstHoleIndex() const
{
    for (int i = 0; i < (int)m_stacks.size(); i++)
    {
        if (m_stacks[i].IsEmpty())
            return i;
    }
    return -1;
}


bool SpiderTableau::IsWon() const
{
    return GetHoleCount() == NUM_STACKS;
}

// for debugging.  Allows to read out tableau positions in the debugger.
void SpiderTableau::MakeStackStrings()
{
    for (auto& stack : m_stacks)
        stack.GetStackString();
    m_stock.GetStackString();
    m_completedPacks.GetStackString();
}


int SpiderTableau::MoveStackSize() const
{
    return (int)m_moveStack.size();
}

int SpiderTableau::GetSavePoint()
{
    return MoveStackSize();
}

bool SpiderTableau::RestoreToSavePoint(int index)
{
    if (index > MoveStackSize())
        return false;

    if (index == MoveStackSize())
        return true;
    auto start = MoveStackSize() - 1;
    for (int m = MoveStackSize() - 1; m >= index; --m)
    {
        UndoMove(m_moveStack[m]);
    }
    m_moveStack.resize(index);
    return true;
}

SpiderTableau::SavePoint::SavePoint(SpiderTableau& tableau)
{
    m_tableau = &tableau;
    m_index = tableau.GetSavePoint();
}

SpiderTableau::SavePoint::~SavePoint()
{
    m_tableau->RestoreToSavePoint(m_index);
}

bool SpiderTableau::operator==(const SpiderTableau& rhs) const
{
    // Comparing the Stack is enough for equality.

    // First compare the sizes of the stacks.
    for (int i = 0; i < NUM_STACKS; ++i)
    {
        if (m_stacks[i].Count() != rhs.m_stacks[i].Count())
            return false;
    }

    // If all the stacks are each the same size then
    // check the stacks in detail.
    for (int i = 0; i < NUM_STACKS; ++i)
    {
        if (m_stacks[i] != rhs.m_stacks[i])
            return false;
    }
    return true;
}


bool SpiderTableau::operator!=(const SpiderTableau& rhs) const
{
    return !(*this == rhs);
}


const std::array<SpiderStack, SpiderTableau::NUM_STACKS>& SpiderTableau::GetStacks() const
{
    return m_stacks;
}


const SpiderStack& SpiderTableau::GetStack(int index) const
{
    return m_stacks[index];
}


std::array<SpiderStack, SpiderTableau::NUM_STACKS>& SpiderTableau::GetMutableStacks()
{
    return m_stacks;
}


int SpiderTableau::IndexOf(const SpiderStack& stack) const
{
    int idx = (int)(& stack - &m_stacks[0]);

    // If the caller doesn't pass a reference to the stack
    // ie. if they pass a copy of the stack the result will be wildly wrong.
    assert(idx >= 0 && idx < NUM_STACKS);
    return (idx);
}


void SpiderTableau::RemoveFullPack(SpiderStack& stack)
{
    m_completedPacks.MoveCards(stack, stack.Count() - 13);
}


void SpiderTableau::ReturnFullPack(SpiderStack& stack)
{
    stack.MoveCards(m_completedPacks, m_completedPacks.Count() - 13);
}


const std::string& SpiderTableau::GetTableauString() const
{
    if (m_string.size() == 0)
        m_string = SpiderString::Create(*this);
    return m_string;
}

