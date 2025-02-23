#pragma once

#include "spidersolvercore/Model/Card.h"
#include "spidersolvercore/Model/SpiderString.h"

#include <vector>

class SpiderStack
{
public:
    SpiderStack();
    SpiderStack(const std::vector<Card>& cards);
    SpiderStack(const SpiderStack& stack);

    SpiderStack& operator=(const SpiderStack& stack);

    ~SpiderStack();

    void SetIsColumn();

    int Count() const;
    bool IsEmpty() const;
    bool IsEmptyOrTopCardNotFaceUp() const;

    void AddNewCard(const Card& card);
    void MoveCard(SpiderStack& srcStack);
    void MoveCards(SpiderStack& srcStack, int cardIndex);

    const Card& PeekTopCard() const;

    bool WouldTurnUpTopCard() const;
    bool TurnUpTopCard();

    bool TurnDownTopCard();

    const std::vector<Card>& GetCards() const;

    const Card& GetCard(int index) const;
    int IndexOf(const Card& card) const;

    int GetHeadMostCardIndex() const;

    int GetRunCount() const;
    int GetRunHead(int deep) const;
    int GetRunTail(int deep) const;

    bool CanRemoveFullPack() const;

    int CountDownCards() const;
    int CountUpCards() const;

    bool operator==(const SpiderStack& rhs) const;
    bool operator!=(const SpiderStack& rhs) const;

    const std::string& GetStackString() const;

    // for debugging
    std::vector<Card>& GetMutableCards();


private:
    void Resize(int cardIndex);
    std::vector<int>& GetRunHeads() const;

private:
    bool m_isColumn;
    std::vector<Card> m_cards;
    mutable std::vector<int> m_heads;
    mutable std::string m_string;
};
