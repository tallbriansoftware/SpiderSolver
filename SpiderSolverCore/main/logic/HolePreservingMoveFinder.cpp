#include "spidersolvercore/logic/HolePreservingMoveFinder.h"

#include "spidersolvercore/Model/MoveCombo.h"
#include "spidersolvercore/Model/SpiderTableau.h"
#include "spidersolvercore/logic/FindStacks.h"
#include "spidersolvercore/logic/SingleMoveFindingFuncs.h"

#include <assert.h>


namespace
{
    void append(std::vector<MoveCombo>& dest, std::vector<MoveCombo>& src)
    {
        dest.insert(dest.end(), src.begin(), src.end());
    }
}

std::vector<MoveCombo> HolePreservingMoveFinder::AllMoves(const SpiderTableau& tableau)
{
    if (tableau.GetHoleCount() == 0)
        return {};

    std::vector<MoveCombo> allMoves;
    std::vector<MoveCombo> moves;
    int moveCount = 0;

    moveCount += AddMoveTwoRuns(allMoves, tableau);
    moveCount += AddFlipRuns(allMoves, tableau);
    moveCount += AddInsertRun(allMoves, tableau);
    moveCount += AddRemoveMiddleRun(allMoves, tableau);
    moveCount += AddTradeHolesA(allMoves, tableau);
    moveCount += AddTradeHolesB(allMoves, tableau);
    moveCount += AddSwapRuns(allMoves, tableau);

    return allMoves;
}

namespace
{
    struct ColumnPair
    {
        int src;
        int dest;
        bool suited;

        ColumnPair(int _src, int _dest)
            :src(_src), dest(_dest), suited(false) {}

        ColumnPair(int _src, int _dest, bool _suited)
            :src(_src), dest(_dest), suited(_suited) {}
    };

    enum class StkSide { Src, Dest };
    struct RunCompare
    {
        StkSide dir0;
        int runIndex0;
        StkSide dir1;
        int runIndex1;
    };

    // Can the srcStack(idx) be placed on the destDatck(destIdx)
    //
    std::vector<ColumnPair> FindFits(
        const SpiderTableau& tableau,
        const std::vector<int>& srcStkNos,
        const std::vector<int>& destStkNos,
        int srcHeadIdx, int destTailIdx)
    {
        std::vector<ColumnPair> stkMoves;

        for (int srcNo : srcStkNos)
        {
            auto srcStack = tableau.GetStack(srcNo);
            Card srcHeadCard = srcStack.GetCard(srcStack.GetRunHead(srcHeadIdx));

            for (int destNo : destStkNos)
            {
                if (srcNo == destNo)
                    continue;

                auto destStack = tableau.GetStack(destNo);
                Card destTailCard = destStack.GetCard(destStack.GetRunTail(destTailIdx));

                if ((int)destTailCard.getRank() == (int)srcHeadCard.getRank() + 1)
                {
                    stkMoves.push_back({ srcNo, destNo });
                }
            }
        }
        return stkMoves;
    }
}

// 1. MoveTwo:  Move two runs 
// |?| |?| |X|         |?| |?| |X|
// |A| |C|      B->X       |C|
// |B|          A->C       |A| 
//             @B->A       |B|
//
// Check that C-A and A-B   dest.0 - src.1  and  src.1 -> src.0 
//
int HolePreservingMoveFinder::AddMoveTwoRuns(
    std::vector<MoveCombo>& moves,
    const SpiderTableau& tableau)
{
    int holeNo = tableau.FindFirstHoleIndex();
    if (holeNo == -1)
        return 0;

    int startCount = (int)moves.size();
    std::vector<int> srcStackNos = FindStacks::RunPattern(tableau, 2, Exactly::No, Sequential::Yes);

    for (int srcNo : srcStackNos)
    {
        auto& src = tableau.GetStack(srcNo);
        int headIndex0 = src.GetRunHead(0);
        int headIndex1 = src.GetRunHead(1);
        assert(headIndex1 != -1);

        Rank srcHeadRank = src.GetCard(headIndex1).getRank();
        std::vector<int> destStackNos = FindStacks::ThatWillRecieveRank(srcHeadRank, tableau);
        for (int destNo : destStackNos)
        {
            if (srcNo == destNo)
                continue;

            int destCount = tableau.GetStack(destNo).Count();
            int sizeOfRun1 = headIndex0 - headIndex1;

            std::vector<MoveSingle> smoves =
            {
                MoveSingle(srcNo, headIndex0, holeNo, 0),
                MoveSingle(srcNo, headIndex1, destNo, destCount),
                MoveSingle(holeNo, 0, destNo, destCount + sizeOfRun1)
            };

            MoveCombo move(smoves);
            move.SetComboType(ComboType::Move2Runs);
            moves.push_back(move);
        }
    }
    return (int)moves.size() - startCount;
}

// 2. FlipRuns: Move two runs into a hole.  Fliping their order and creating a hole.
// |X| |X|       |X| |X|
// |A|     B->X      |B|
// |B|    @A->B      |A|
//
// Check that B-A
//
int HolePreservingMoveFinder::AddFlipRuns(
    std::vector<MoveCombo>& moves,
    const SpiderTableau& tableau)
{
    int holeNo = tableau.FindFirstHoleIndex();
    if (holeNo == -1)
        return 0;

    int startCount = (int)moves.size();
    std::vector<int> srcStackNos = FindStacks::RunPattern(tableau, 2, Exactly::Yes);

    for (int srcNo : srcStackNos)
    {
        auto& src = tableau.GetStack(srcNo);
        int headIndex1 = src.GetRunHead(1);
        int tailIndex0 = src.GetRunTail(0);
        assert(headIndex1 != -1);

        int headVal1 = (int)src.GetCard(headIndex1).getRank();
        int tailVal0 = (int)src.GetCard(tailIndex0).getRank();

        // There must be a gap to fill between the tail1 and head0.
        if (tailVal0 == headVal1 + 1)
        {
            int headIndex0 = src.GetRunHead(0);
            int sizeOfRun0 = tailIndex0 - headIndex0 + 1;

            std::vector<MoveSingle> smoves =
            {
                MoveSingle(srcNo, headIndex0, holeNo, 0),
                MoveSingle(srcNo, headIndex1, holeNo, sizeOfRun0),
            };

            MoveCombo move(smoves);
            move.SetComboType(ComboType::FlipRuns);
            moves.push_back(move);
        }
    }
    return (int)moves.size() - startCount;
}

// 3. InsertRun:  Insert a run between two existing runs.
// |?| |?| |X|        |?| |?| |X|
// |A| |C|      B->X  |A|
// |B|          C->A  |C|
//             @B->C  |B|
//
// Check that A-C and C-B  ie.  dest.1 - src.0  and src.0 - dest.0
//
int HolePreservingMoveFinder::AddInsertRun(
    std::vector<MoveCombo>& moves,
    const SpiderTableau& tableau)
{
    int holeNo = tableau.FindFirstHoleIndex();
    if (holeNo == -1)
        return 0;

    int startCount = (int)moves.size();
    std::vector<int> targetStackNos = FindStacks::RunPattern(tableau, 2);

    for (int targetNo : targetStackNos)
    {
        // Confirm that the two runs are in order but have a gap.
        auto& target = tableau.GetStack(targetNo);
        int tailIndex1 = target.GetRunTail(1);
        int headIndex0 = target.GetRunHead(0);
        assert(tailIndex1 != -1);

        int tailVal1 = (int)target.GetCard(tailIndex1).getRank();
        int headVal0 = (int)target.GetCard(headIndex0).getRank();

        // There must be a gap to fill between the tail1 and head0.
        if (tailVal1 <= headVal0 + 1)
            continue;

        // Find candidates to fill the gap.
        std::vector<int> srcStackNos = FindStacks::RunPattern(tableau, 1);
        for (int srcNo : srcStackNos)
        {
            if (srcNo == targetNo)
                continue;
            auto& src = tableau.GetStack(srcNo);
            int srcHead = src.GetRunHead(0);
            int srcTail = src.GetRunTail(0);
            assert(srcHead != -1);

            int headVal = (int)src.GetCard(srcHead).getRank();
            int tailVal = (int)src.GetCard(srcTail).getRank();

            if (headVal + 1 == tailVal1 && tailVal == headVal0 + 1)
            {
                int sizeOfSrcRun = srcTail - srcHead + 1;

                std::vector<MoveSingle> smoves =
                {
                    MoveSingle(targetNo, headIndex0, holeNo, 0),
                    MoveSingle(srcNo, srcHead, targetNo, headIndex0),
                    MoveSingle(holeNo, 0, targetNo, headIndex0 + sizeOfSrcRun)
                };
                MoveCombo move(smoves);
                move.SetComboType(ComboType::InsertRun);
                moves.push_back(move);
            }
        }
    }
    return (int)moves.size() - startCount;
}

// 4. RemoveMiddleRun:  Remove run from between two runs.
// |?| |?| |X|         |?| |?| |X|
// |A| |D|      C->X   |A| |D|
// |B|          B->D   |C| |B| 
// |C|         @C->A
//
// Check that A-C and D-B  ie  src.2 - src.0  and dest.0 - src.1
//
int HolePreservingMoveFinder::AddRemoveMiddleRun(
    std::vector<MoveCombo>& moves,
    const SpiderTableau& tableau)
{
    int holeNo = tableau.FindFirstHoleIndex();
    if (holeNo == -1)
        return 0;

    int startCount = (int)moves.size();
    std::vector<int> srcStackNos = FindStacks::RunPattern(tableau, 3);

    for (int srcNo : srcStackNos)
    {
        auto& src = tableau.GetStack(srcNo);
        int headIndex0 = src.GetRunHead(0);
        int tailIndex2 = src.GetRunTail(2);
        Card card0 = src.GetCard(headIndex0);
        Card card2 = src.GetCard(tailIndex2);
        if ((int)card2.getRank() != (int)card0.getRank() + 1)
            continue;

        std::vector<int> targetStackNos = FindStacks::RunPattern(tableau, 1);
        for (int targetNo : targetStackNos)
        {
            if (targetNo == srcNo)
                continue;

            auto& target = tableau.GetStack(targetNo);
            int headIndex1 = src.GetRunHead(1);
            int tailIndex = target.GetRunTail(0);
            Card headCard = src.GetCard(headIndex1);
            Card tailCard = target.GetCard(tailIndex);
            if ((int)headCard.getRank() + 1 != (int)tailCard.getRank())
                continue;

            int targetCount = target.Count();

            std::vector<MoveSingle> smoves =
            {
                MoveSingle(srcNo, headIndex0, holeNo, 0),
                MoveSingle(srcNo, headIndex1, targetNo, targetCount),
                MoveSingle(holeNo, 0, srcNo, tailIndex2 + 1)
            };

            MoveCombo move(smoves);
            move.SetComboType(ComboType::RemoveMiddleRun);
            moves.push_back(move);
        }
    }
    return (int)moves.size() - startCount;
}

// 5. TradeHoles-A: Fill a hole to make a hole.
//  |X| |?| |X|       |X| |?| |X|
//  |A| |B|     C->X      |B| |C|
//      |C|    @A->B      |A|
//
// Check that B-A  ie.  dest.1 - src.0
//
int HolePreservingMoveFinder::AddTradeHolesA(
    std::vector<MoveCombo>& moves, 
    const SpiderTableau& tableau)
{
    int holeNo = tableau.FindFirstHoleIndex();
    if (holeNo == -1)
        return 0;

    int startCount = (int)moves.size();
    std::vector<int> srcStackNos = FindStacks::RunPattern(tableau, 1, Exactly::Yes);
    std::vector<int> destStackNos = FindStacks::RunPattern(tableau, 2);

    for (int srcStkNo : srcStackNos)
    {
        auto srcStack = tableau.GetStack(srcStkNo);
        Card headOfSingleRunCard = srcStack.GetCard(srcStack.GetRunHead(0));

        for (int destStkNo : destStackNos)
        {
            auto multipleRunsStack = tableau.GetStack(destStkNo);
            Card tailOfSecondRunCard = multipleRunsStack.GetCard(multipleRunsStack.GetRunTail(1));
            if ((int)tailOfSecondRunCard.getRank() == (int)headOfSingleRunCard.getRank() + 1)
            {
                int multipleRunsHeadIndex = multipleRunsStack.GetRunHead(0);

                std::vector<MoveSingle> smoves =
                {
                    MoveSingle(destStkNo, multipleRunsHeadIndex, holeNo, 0),
                    MoveSingle(srcStkNo, 0, destStkNo, multipleRunsHeadIndex)
                };
                MoveCombo move(smoves);
                move.SetComboType(ComboType::TradeHolesA);
                moves.push_back(move);
            }
        }
    }
    return (int)moves.size() - startCount;
}

// 6. TradeHoles-B: Fill a hole to make a hole.
//  |X| |?| |X|       |X| |?| |X|
//  |A| |C|     B->X      |C| |B|
//  |B|        @A->C      |A|
//
// Check that C-A  ie.  dest.0 - src.1
//
int HolePreservingMoveFinder::AddTradeHolesB(
    std::vector<MoveCombo>& moves, 
    const SpiderTableau& tableau)
{
    int holeNo = tableau.FindFirstHoleIndex();
    if (holeNo == -1)
        return 0;

    int startCount = (int)moves.size();
    std::vector<int> srcStackNos = FindStacks::RunPattern(tableau, 2, Exactly::Yes);
    std::vector<int> destStackNos = FindStacks::RunPattern(tableau, 1);
    auto stkMoves = FindFits(tableau, srcStackNos, destStackNos, 1, 0);

    for (auto& pair : stkMoves)
    {
        auto srcStack = tableau.GetStack(pair.src);
        auto destStack = tableau.GetStack(pair.dest);
        int srcRun0 = srcStack.GetRunHead(0);
        int srcRun1 = srcStack.GetRunHead(1);
        int destTail = destStack.Count();

        std::vector<MoveSingle> smoves =
        {
            MoveSingle(pair.src, srcRun0, holeNo, 0),
            MoveSingle(pair.src, srcRun1, pair.dest, destTail)
        };
        MoveCombo move(smoves);
        move.SetComboType(ComboType::TradeHolesB);
        moves.push_back(move);
    }
    return (int)moves.size() - startCount;
}

// 7. SwapRuns: Swap two runs:
// |?| |?| |X|  D->X  |?| |?| |X|
// |A| |C|      B->C  |A| |C|
// |B| |D|     @D->A  |D| |B|
// 
// check that A-D and C-B and at least one is suited.
//
int HolePreservingMoveFinder::AddSwapRuns(
    std::vector<MoveCombo>& moves,
    const SpiderTableau& tableau)
{
    int holeNo = tableau.FindFirstHoleIndex();
    if (holeNo == -1)
        return 0;

    int startCount = (int)moves.size();
    std::vector<int> multipleRunsStackNos = FindStacks::RunPattern(tableau, 2);

    for (int stk1 = 0; stk1 < SpiderTableau::NUM_STACKS; stk1++)
    {
        auto stack1 = tableau.GetStack(stk1);
        if (stack1.IsEmpty() || stack1.GetRunCount() <= 1)
            continue;
        Card destCard1 = stack1.GetCard(stack1.GetRunTail(1));
        Card srcCard1 = stack1.GetCard(stack1.GetRunHead(0));
        if ((int)srcCard1.getRank() + 1 != (int)destCard1.getRank())
            continue;

        for (int stk2 = stk1 + 1; stk2 < SpiderTableau::NUM_STACKS; stk2++)
        {
            if (stk1 == stk2)
                continue;
            auto stack2 = tableau.GetStack(stk2);
            if (stack2.IsEmpty() || stack2.GetRunCount() <= 1)
                continue;
            Card destCard2 = stack2.GetCard(stack2.GetRunTail(1));
            Card srcCard2 = stack2.GetCard(stack2.GetRunHead(0));

            // The rank of the head of the runs must be the same.
            // and the rank of the dest tails must be the same.
            if ((destCard1.getRank() != destCard2.getRank()) ||
                (srcCard1.getRank() != srcCard2.getRank()))
                continue;

            // At least one of the two moves must suit up.
            // In 2 suit they both will suit up
            // In 4 suit possibly one or both or neither.
            // In 1 suit this issue (swaps) never happens.
            // The suit of the destination must match at least one
            // of the runs moving.
            if ((destCard1.getSuit() != srcCard2.getSuit()) &&
                (destCard2.getSuit() != srcCard1.getSuit()))
                continue;

            int idx1 = stack1.GetRunHead(0);
            int idx2 = stack2.GetRunHead(0);
            std::vector<MoveSingle> smoves =
            {
                MoveSingle(stk1, idx1, holeNo, 0),
                MoveSingle(stk2, idx2, stk1, idx1),
                MoveSingle(holeNo, 0, stk2, idx2)
            };
            MoveCombo move(smoves);
            move.SetComboType(ComboType::SwapRuns);
            moves.push_back(move);
        }
    }
    return (int)moves.size() - startCount;
}
