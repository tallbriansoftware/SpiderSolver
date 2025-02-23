#pragma once

#include "spidersolvercore/Model/MoveSingle.h"

class SpiderTableau;

//
// Extend a MoveSingle object to have enough information to undo the move.
// 
class MoveReversible
{
public:
    MoveReversible();
    MoveReversible(const MoveSingle& move, const SpiderTableau& stack);

    int FromStack() const;
    int FromIndex() const;
    int DestStack() const;
    int DestIndex() const;

    bool TurnedCard() const;
    bool RemovedPack() const;
    bool PackTurnedCard() const;


private:
    MoveSingle m_move;

    bool m_turnedCard;
    bool m_removedPack;
    bool m_removedPackTurnedCard;
};
