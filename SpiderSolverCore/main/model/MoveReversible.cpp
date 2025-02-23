#include "spidersolvercore/Model/MoveReversible.h"

#include "spidersolvercore/Model/SpiderTableau.h"
#include "spidersolvercore/Model/SpiderStack.h"
#include "spidersolvercore/Model/CardEnums.h"

MoveReversible::MoveReversible()
    : m_move()
    , m_turnedCard(false)
    , m_removedPack(false)
    , m_removedPackTurnedCard(false)
{
}

MoveReversible::MoveReversible(const MoveSingle& move, const SpiderTableau& tableau)
    :m_move(move)
    , m_turnedCard(false)
    , m_removedPack(false)
    , m_removedPackTurnedCard(false)
{
    auto& src = tableau.GetStack(move.FromStack());
    int fromIndex = move.FromIndex();

    if (fromIndex > 0 && !src.GetCard(fromIndex - 1).IsFaceUp())
        m_turnedCard = true;

    auto& dest = tableau.GetStack(move.DestStack());
    if (!dest.IsEmpty())
    {
        int destHeadIndex = dest.GetRunHead(0);

        Card srcTailCard = src.PeekTopCard();
        Card destHeadCard = dest.GetCard(destHeadIndex);

        if (srcTailCard.getRank() == Rank::Ace
            && destHeadCard.getRank() == Rank::King
            && srcTailCard.getSuit() == destHeadCard.getSuit())
        {
            m_removedPack = true;

            if (destHeadIndex > 0 && !dest.GetCard(destHeadIndex - 1).IsFaceUp())
                m_removedPackTurnedCard = true;
        }
    }
}


int MoveReversible::FromStack() const
{
    return m_move.FromStack();
}

int MoveReversible::FromIndex() const
{
    return m_move.FromIndex();
}

int MoveReversible::DestStack() const
{
    return m_move.DestStack();
}

int MoveReversible::DestIndex() const
{
    return m_move.DestIndex();
}


bool MoveReversible::TurnedCard() const
{
    return m_turnedCard;
}

bool MoveReversible::RemovedPack() const
{
    return m_removedPack;
}

bool MoveReversible::PackTurnedCard() const
{
    return m_removedPackTurnedCard;
}

