#include "spidersolvercore/logic/IsMoveLegal.h"

#include "spidersolvercore/model/MoveCombo.h"
#include "spidersolvercore/model/MoveSingle.h"
#include "spidersolvercore/model/SpiderTableau.h"


#ifdef _DEBUG
bool IsMoveLegal(const SpiderTableau& tableau, int fromStack, int fromIndex, int destStack)
{
    int destIndex = tableau.GetStack(destStack).Count();
    return IsMoveLegal(tableau, fromStack, fromIndex, destStack, destIndex);
}


bool IsMoveLegal(const SpiderTableau& tableau, int fromStack, int fromIndex, int destStack, int destIndex)
{
    auto& src = tableau.GetStack(fromStack);
    auto& dest = tableau.GetStack(destStack);

    if (fromIndex >= src.Count())
        return false;

    if (fromIndex < src.GetRunHead(0))
        return false;

    if (destIndex != dest.Count())
        return false;

    auto& srcHeadCard = src.GetCard(fromIndex);

    if (dest.IsEmpty())
        return true;

    auto& destTopCard = dest.PeekTopCard();
    return (int)srcHeadCard.getRank() + 1 == (int)destTopCard.getRank();
}

bool IsMoveLegal(const SpiderTableau& tableau, const MoveSingle& smove)
{
    return IsMoveLegal(tableau, smove.FromStack(), smove.FromIndex(), smove.DestStack(), smove.DestIndex());
}

bool IsMoveLegal(const SpiderTableau& tableau, const MoveCombo& move)
{
    SpiderTableau scratch(tableau);
    for (const auto& smove : move.GetMoves())
    {
        if (!IsMoveLegal(scratch, smove))
            return false;
        scratch.DoMove(smove, DoTurnCard::No);
    }
    return true;
}
#endif
