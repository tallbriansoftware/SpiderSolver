#include "spidersolvercore/logic/FindStacks.h"

#include "spidersolvercore/model/SpiderTableau.h"


// Find Stacks that you can play a "rank" card on.
//
std::vector<int> FindStacks::ThatWillRecieveRank(Rank rank, const SpiderTableau& tableau)
{
    std::vector<int> destStackNos;
    for (auto& stack : tableau.GetStacks())
    {
        if (stack.IsEmptyOrTopCardNotFaceUp())
            continue;

        Rank topRank = stack.PeekTopCard().getRank();
        if ((int)topRank == (int)rank + 1)
            destStackNos.push_back(tableau.IndexOf(stack));
    }
    return destStackNos;
}

// Find stacks that you can play a range of "ranks" cards on.
std::vector<int> FindStacks::ThatWillRecieveRankInRange(
    Rank rankLower,
    Rank rankUpper,
    const SpiderTableau& tableau)
{
    std::vector<int> destStkNos;
    for (auto& dest : tableau.GetStacks())
    {
        if (dest.IsEmptyOrTopCardNotFaceUp())
            continue;

        Rank destTopRank = dest.PeekTopCard().getRank();

        // if the destTopRank is 9 then the range has to be 9 or greater to below 9.
        // If the range is 9-5 then the dest top Rank needs to greater than 5, and 9 or less.
        // so that some of the range can be placed on destTopRank.
        // We don't want a destTopRank of Ten(T) because that is an ordinary good move (handled elsewhere).
        //
        if ((int)destTopRank > (int)rankLower && (int)destTopRank <= (int)rankUpper)
            destStkNos.push_back(tableau.IndexOf(dest));
    }
    return destStkNos;
}


// RunPattern (stack)
// Returns true/false if the stack has a
// requested number of suited runs.
// At least or exactly.
bool FindStacks::RunPattern(
    const SpiderStack& stack,
    int numberOfRuns,
    Exactly exactly,
    Sequential seq)
{
    if (stack.IsEmptyOrTopCardNotFaceUp())
        return false;

    // There must be N runs (at least)
    if (stack.GetRunCount() < numberOfRuns)
        return false;

    // Exactly Equal means it will leave a hole.
    if (exactly == Exactly::Equal)
    {
        // Must have N runs (exactly)
        if (stack.GetRunCount() != numberOfRuns)
            return false;
        // and no down cards.
        // exactly also means no down cards.
        if (stack.CountDownCards() != 0)
            return false;
    }

    if (seq == Sequential::Yes && numberOfRuns > 1)
    {
        bool isSeq = true;
        for (int i = 0; i < numberOfRuns - 1; i++)
        {
            int index1 = stack.GetRunHead(i);
            int index2 = stack.GetRunTail(i + 1);
            Card card1 = stack.GetCard(index1);
            Card card2 = stack.GetCard(index2);
            if ((int)card2.getRank() != (int)card1.getRank() + 1)
            {
                isSeq = false;
                break;
            }
        }
        if (!isSeq)
            return false;
    }
    return true;

}

// RunPattern (tabeau)
// Collect the stack Numbers that pass
// RunPattern(stack) test.
std::vector<int> FindStacks::RunPattern(
    const SpiderTableau& tableau,
    int numberOfRuns,
    Exactly exactly,
    Sequential seq)
{
    std::vector<int> stackNumbers;

    for (int stackNo = 0; stackNo < SpiderTableau::NUM_STACKS; stackNo++)
    {
        if (RunPattern(tableau.GetStack(stackNo), numberOfRuns, exactly, seq))
            stackNumbers.push_back(stackNo);
    }
    return stackNumbers;
}
