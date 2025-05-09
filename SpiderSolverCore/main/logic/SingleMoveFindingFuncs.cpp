#include "spidersolvercore/Logic/SingleMoveFindingFuncs.h"

#include "spidersolvercore/Model/SpiderStack.h"
#include "spidersolvercore/Model/SpiderTableau.h"


int SingleMoveFindingFuncs::FindMoves(
    std::vector<MoveSingle>& moves,
    const SpiderTableau& tableau,
    SingleMoveFindingFuncs::FindingFunction findingFunc)
{
    int count = 0;
    for (auto& srcStack : tableau.GetStacks())
    {
        for (auto& destStack : tableau.GetStacks())
        {
            if (&srcStack != &destStack)
            {
                if (srcStack.IsEmpty() || !srcStack.PeekTopCard().IsFaceUp())
                    continue;
                if (!destStack.IsEmpty() && !destStack.PeekTopCard().IsFaceUp())
                    continue;

                int srcIndex = findingFunc(srcStack, destStack);
                int destIndex = destStack.Count();
                if (srcIndex != -1)
                {
                    MoveSingle smove(
                        tableau.IndexOf(srcStack), srcIndex,
                        tableau.IndexOf(destStack), destIndex);
                    moves.emplace_back(smove);
                    count += 1;
                }
            }
        }
    }
    return count;
}


int SingleMoveFindingFuncs::FindMovesForDestinationStack(
    std::vector<MoveSingle>& moves,
    const SpiderStack& destStack,
    const SpiderTableau& tableau,
    SingleMoveFindingFuncs::FindingFunction findingFunc)
{
    int count = 0;
    if (!destStack.IsEmpty() && !destStack.PeekTopCard().IsFaceUp())
        return count;

    for (auto& srcStack : tableau.GetStacks())
    {
        if (&srcStack != &destStack)
        {
            if (srcStack.IsEmpty() || !srcStack.PeekTopCard().IsFaceUp())
                continue;

            int srcIndex = findingFunc(srcStack, destStack);
            int destIndex = destStack.Count();
            if (srcIndex != -1)
            {
                MoveSingle smove(
                    tableau.IndexOf(srcStack), srcIndex,
                    tableau.IndexOf(destStack), destIndex);
                moves.emplace_back(smove);
                count += 1;
            }
        }
    }
    return count;
}


// SimpleMoves:  Find normal simple moves.
// -- Excludes pluging holes
// -- Excludes breaking suited run
//
int SingleMoveFindingFuncs::SimpleMove(const SpiderStack& src, const SpiderStack& dest)
{
    int srcIndex = src.GetRunHead(0);
    int destIndex = dest.GetRunTail(0);
    if (srcIndex < 0 || destIndex < 0)
        return -1;

    const auto& fromCard = src.GetCard(srcIndex);
    const auto& destCard = dest.GetCard(destIndex);

    if ((int)fromCard.getRank() + 1 != (int)destCard.getRank())
        return -1;

    return srcIndex;
}


// ColorUpMoves:  Find moves that make suited runs longer.
// -- of course this allows the breaking up of other suited runs.
//
int SingleMoveFindingFuncs::ColorUpMove(const SpiderStack& src, const SpiderStack& dest)
{
    int srcHeadIndex = src.GetRunHead(0);
    int destHeadIndex = dest.GetRunHead(0);
    if (srcHeadIndex == -1 || destHeadIndex == -1)
        return -1;

    const Card& srcHeadCard = src.GetCard(srcHeadIndex);

    // assuming that if the head values are good
    // the tail values will be good.
    int srcTailIndex = src.GetRunTail(0);
    int destTailIndex = dest.GetRunTail(0);

    const Card& srcTailCard = src.GetCard(srcTailIndex);
    const Card& destTailCard = dest.GetCard(destTailIndex);

    // They must be the same suit.
    if (srcTailCard.getSuit() != destTailCard.getSuit())
        return -1;

    // The dest-tail rank must be between the src-head and src-tail ranks.
    // otherwise there is nothing to move.
    if (srcTailCard.getRank() >= destTailCard.getRank())
        return -1;

    // if it is exactly 1 less then it is a normal suited move (covered elsewhere)
    // if it is more than 1 less than it is not a legal move.
    if (srcHeadCard.getRank() < destTailCard.getRank())
        return -1;

    // lastly chech that the dest result is a longer run than source
    int srcRunLength = srcTailIndex - srcHeadIndex + 1;
    int destRunLength = destTailIndex - destHeadIndex + 1;
    int countToMove = (int)destTailCard.getRank() - (int)srcTailCard.getRank();
    if (destRunLength + countToMove <= srcRunLength)
        return -1;

    return srcTailIndex - countToMove + 1;
}

#ifdef _DEBUG
//
// This is like "ColorUp" but the dest does not have to be the same suit.
// Not generally usefull except when clearing stacks.
//
int SingleMoveFindingFuncs::SuitedRunSplittingMove(const SpiderStack& src, const SpiderStack& dest)
{
    int srcHeadIndex = src.GetRunHead(0);
    int destHeadIndex = dest.GetRunHead(0);
    if (srcHeadIndex == -1 || destHeadIndex == -1)
        return -1;

    const Card& srcHeadCard = src.GetCard(srcHeadIndex);

    // assuming that if the head values are good
    // the tail values will be good.
    int srcTailIndex = src.GetRunTail(0);
    int destTailIndex = dest.GetRunTail(0);

    const Card& srcTailCard = src.GetCard(srcTailIndex);
    const Card& destTailCard = dest.GetCard(destTailIndex);

    // They must be different suits.
    if (srcTailCard.getSuit() == destTailCard.getSuit())
        return -1;

    // The dest-tail rank must be between the src-head and src-tail ranks.
    // otherwise there is nothing to move.
    if (srcTailCard.getRank() >= destTailCard.getRank())
        return -1;

    // if it is exactly 1 less then it is a normal move, not a splitter.
    // It is covered elsewhere.
    // if it is more than 1 less than it is not a legal move.
    if ((int)srcHeadCard.getRank() < (int)destTailCard.getRank())
        return -1;

    int countToMove = (int)destTailCard.getRank() - (int)srcTailCard.getRank();

    return srcTailIndex - countToMove + 1;
}
#endif


int SingleMoveFindingFuncs::HoleFillingMove(const SpiderStack& src, const SpiderStack& dest)
{
    if (!dest.IsEmpty())
        return -1;

    int srcHeadIndex = src.GetRunHead(0);
    if (srcHeadIndex < 0)
        return -1;

    return srcHeadIndex;
}
