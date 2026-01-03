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
    std::vector<int> destStackNos;
    for (auto& stack : tableau.GetStacks())
    {
        if (stack.IsEmptyOrTopCardNotFaceUp())
            continue;

        Rank topRank = stack.PeekTopCard().getRank();

        // if the range is 6-9 then topRank must be 6, 7, or 8
        // so that some of the range can be placed on topRank.
        // we don't want a topRank of 5 because that is an ordinary good move.
        if ((int)rankLower >= (int)topRank && (int)topRank < (int)rankUpper)
            destStackNos.push_back(tableau.IndexOf(stack));
    }
    return destStackNos;
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
