#include "gtest/gtest.h"

#include "spidersolvercore/Logic/HolePreservingMoveFinder.h"
#include "spidersolvercore/Model/SpiderTableau.h"
#include "spidersolvercore/utils/SpiderPrint.h"

#include "Helpers/DeckHelper.h"

namespace
{
    void PrintTableau(const SpiderTableau& tableau)
    {
        for (auto& line : SpiderPrint::PrintTableau(tableau))
            std::cout << line << std::endl;
    }

    void PrintMoves(const SpiderTableau& tableau, const std::vector<MoveCombo>& moves)
    {
        // output moves with scores
        for (auto& move : moves)
        {
            std::string moveString = SpiderPrint::PrintBookMove(tableau, move, DoTurnCard::No);
            std::cout << moveString << std::endl;
        }
    }


    void SetStack(SpiderStack& stack, std::vector<Card> cards)
    {
        for (auto card : cards)
            stack.AddNewCard(card);
    }

    bool IsSameMove(const MoveSingle& expected, const MoveSingle& actual)
    {
        EXPECT_EQ(expected.FromStack(), actual.FromStack());
        EXPECT_EQ(expected.FromIndex(), actual.FromIndex());
        EXPECT_EQ(expected.DestStack(), actual.DestStack());
        EXPECT_EQ(expected.DestIndex(), actual.DestIndex());
        return true;
    }

    bool IsSameMove(std::vector<MoveSingle> expected, std::vector<MoveSingle> actual)
    {
        EXPECT_EQ(expected.size(), actual.size());
        if (expected.size() != actual.size())
            return false;
        for (int i = 0; i < actual.size(); i++)
            IsSameMove(expected[i], actual[i]);
        return true;
    }
}

using namespace Cards;

TEST(HolePreservingMoveFinderTests, GeneralTest) {
    SpiderTableau tableau;

    auto& stacks = tableau.GetMutableStacks();

    SetStack(stacks[0],
        { DJS(), DAH(), D4S(), D2H(), DAS(), UAH(), UKH(), UQH(), U5S(), U4S(), U3S(), U2S(), UAH(), UKH(), UQS(), UJH() });
    SetStack(stacks[1],
        { D7S(), D6H(), D5H(), D8S(), UKH(), UQH(), UKS(), UQS(), UJS(), UTS(), U9S(), U9S(), U8S() });
    SetStack(stacks[2],
        { D5H(), D7H(), DKS(), DAH(), UTS(), U9S(), UTH(), U9H(), U8H(), U7H(), U6S(), U5S(), U4S() });
    // Stack 3 is empty.
    SetStack(stacks[4],
        { DKS(), DJS(), D4H(), D5H(), U9H(), U8H(), U3S(), U4H(), U3S(), U2H(), U7S(), U6S(), U5S(), U4H(), U3S(), UJH() });
    SetStack(stacks[5],
        { D8S(), D2S(), D8H(), UKS(), U3H(), UQH(), UTS(), UAS() });
    SetStack(stacks[6],
        { D3H(), D7H(), D6S(), DJH(), UKH(), UQH(), UJH(), UTH(), U9H(), U8H(), U7H(), U6H() });
    SetStack(stacks[7],
        { D7S(), D2S(), D4S(), UAS(), U5H(), U4H(), U3H(), U2H(), UTH(), U9H(), UQS() });
    SetStack(stacks[8],
        { D8S(), D2H(), DJS(), DAS(), U3H(), UTS(), U9S(), U7S(), U6H() });
    SetStack(stacks[9],
        { DQS(), D6H(), U5S(), U2S(), UTH(), U6S() });

/*
0  1  2  3  4  5  6  7  8  9
:: :: ::  - :: :: :: :: :: ::
:: :: ::  - :: :: :: :: :: ::
:: :: ::  - :: :: :: :: :: 5S
:: :: ::  - :: KS :: AS :: 2S
:: KH TS  - 9H 3H KH 5H 3H TH
AH QH 9S  - 8H QH QH 4H TS 6S
KH KS TH  - 3S TS JH 3H 9S  -
QH QS 9H  - 4H AS TH 2H 7S  -
5S JS 8H  - 3S  - 9H TH 6H  -
4S TS 7H  - 2H  - 8H 9H  -  -
3S 9S 6S  - 7S  - 7H QS  -  -
2S 9S 5S  - 6S  - 6H  -  -  -
AH 8S 4S  - 5S  -  -  -  -  -
KH  -  -  - 4H  -  -  -  -  -
QS  -  -  - 3S  -  -  -  -  -
JH  -  -  - JH  -  -  -  -  -
*/
#ifdef _DEBUG
    PrintTableau(tableau);
#endif

    auto hole1_Moves = HolePreservingMoveFinder::AllMoves(tableau);
/*
    2{6S4}<=>8{6H} swap-runs
    0{JH}2{TH7-6S4} Move-2-Seq-Runs
    4{JH}2{TH7-6S4} Move-2-Seq-Runs
    1{9S8}8{7S-6H} Move-2-Seq-Runs
*/
#ifdef _DEBUG
    PrintMoves(tableau, hole1_Moves);
#endif

    for (auto& move : hole1_Moves)
    {
        SpiderTableau::SavePoint save(tableau);

        tableau.DoMove(move, DoTurnCard::Auto);
        EXPECT_EQ(tableau.GetStack(3).Count(), 0);
    }
}

TEST(HolePreservingMoveFinderTests, FlipRuns) {
    SpiderTableau tableau;

    auto& stacks = tableau.GetMutableStacks();

    SetStack(stacks[0], { D2H(), U3S()});
    SetStack(stacks[1], { D3H(), U4S() });
    // This stack is moved because it leaves a hole.
    SetStack(stacks[2], { U5H(), U4H(), U7S(), U6S() });
    SetStack(stacks[3], { D4H(), U5S() });
    SetStack(stacks[4], { D5H(), U6S() });
    // Stack 5 empty.
    SetStack(stacks[6], { D6H(), U7S() });
    // Stack 7 is not moved because it doesn't leave a hole.
    SetStack(stacks[7], { D7H(), U8S(), U7S(), UTS(), U9S() });
    SetStack(stacks[8], { D8H(), U9S() });
    SetStack(stacks[9], { D9H(), UTS() });

/*
 0  1  2  3  4  5  6  7  8  9
:: :: 5H :: ::  - :: :: :: ::
3S 4S 4H 5S 6S  - 7S 8S 9S TS
 -  - 7S  -  -  -  - 7S  -  -
 -  - 6S  -  -  -  - TS  -  -
 -  -  -  -  -  -  - 9S  -  -
 */

#ifdef _DEBUG
    PrintTableau(tableau);
#endif

    std::vector<MoveCombo> moves;
    int count = HolePreservingMoveFinder::AddFlipRuns(moves, tableau);

/*
    5{E0}2{7S6}, 5{7S6}2{5H4=E1} Flip-Runs
*/

#ifdef _DEBUG
    PrintMoves(tableau, moves);
#endif

    EXPECT_EQ(moves.size(), 1);
    EXPECT_EQ(count, 1);
    auto actual = moves[0].GetMoves();

    std::vector<MoveSingle> expected =
    {
        MoveSingle(2, 2, 5, 0),
        MoveSingle(2, 0, 5, 2)
    };

    EXPECT_TRUE(IsSameMove(actual, expected));
}

TEST(HolePreservingMoveFinderTests, TradeHolesA) {
    SpiderTableau tableau;

    auto& stacks = tableau.GetMutableStacks();

    SetStack(stacks[0], { D2H(), U3S() });
    SetStack(stacks[1], { D3H(), U4S() });
    SetStack(stacks[2], { U9H(), U8H(), U7H(), U3S(), U2S() });
    SetStack(stacks[3], { D4H(), U5S() });
    SetStack(stacks[4], { U6S(), U5S(), U4S() });
    // Stack 5 empty.
    SetStack(stacks[6], { D6H(), U7S() });
    // Stack 7 is not moved because it doesn't leave a hole.
    SetStack(stacks[7], { D7H(), U6S(), U5S(), U4S() });
    SetStack(stacks[8], { D8H(), U9S() });
    SetStack(stacks[9], { D9H(), UTS() });

/*
 0  1  2  3  4  5  6  7  8  9
:: :: 9H :: 6S  - :: :: :: ::
3S 4S 8H 5S 5S  - 7S 6S 9S TS
 -  - 7H  - 4S  -  - 5S  -  -
 -  - 3S  -  -  -  - 4S  -  -
 -  - 2S  -  -  -  -  -  -  -
*/

#ifdef _DEBUG
    PrintTableau(tableau);
#endif

    std::vector<MoveCombo> moves;
    int count = HolePreservingMoveFinder::AddTradeHolesA(moves, tableau);
/*
    5{E0}2{3S2}, 2{9H7}4{6S4=E1} Trade-Holes
*/

#ifdef _DEBUG
    PrintMoves(tableau, moves);
#endif

    EXPECT_EQ(moves.size(), 1);
    EXPECT_EQ(count, 1);
    auto actual = moves[0].GetMoves();

    std::vector<MoveSingle> expected =
    {
        MoveSingle(2, 3, 5, 0),
        MoveSingle(4, 0, 2, 3)
    };

    EXPECT_TRUE(IsSameMove(actual, expected));
}

TEST(HolePreservingMoveFinderTests, TradeHolesB) {
    SpiderTableau tableau;

    auto& stacks = tableau.GetMutableStacks();

    SetStack(stacks[0], { D2H(), U3S() });
    SetStack(stacks[1], { D3H(), U4S() });
    SetStack(stacks[2], { U9H(), U8H(), U7H(), U3S(), U2S() });
    SetStack(stacks[3], { D4H(), U5S() });
    SetStack(stacks[4], { D6S(), U8S(), U7S() });
    // Stack 5 empty.
    SetStack(stacks[6], { D6H(), U7S() });
    // Stack 7 is not moved because it doesn't leave a hole.
    SetStack(stacks[7], { D7H(), U6S(), U5S(), U4S() });
    SetStack(stacks[8], { D8H(), U9S() });
    SetStack(stacks[9], { D9H(), UTS() });

/*
   0  1  2  3  4  5  6  7  8  9
0 :: :: 9H :: 6S  - :: :: :: ::
1 3S 4S 8H 5S 5S  - 7S 6S 9S TS
2  -  - 7H  - 4S  -  - 5S  -  -
3  -  - 3S  -  -  -  - 4S  -  -
4  -  - 2S  -  -  -  -  -  -  -
*/

#ifdef _DEBUG
    PrintTableau(tableau);
#endif

    std::vector<MoveCombo> moves;
    int count = HolePreservingMoveFinder::AddTradeHolesB(moves, tableau);
    /*
        5{E0}2{3S2}, 2{9H7}4{6S4=E1} Trade-Holes
    */

#ifdef _DEBUG
    PrintMoves(tableau, moves);
#endif

    EXPECT_EQ(moves.size(), 1);
    EXPECT_EQ(count, 1);
    if (moves.size() > 0)
    {
        auto actual = moves[0].GetMoves();

        std::vector<MoveSingle> expected =
        {
            MoveSingle(2, 3, 5, 0),
            MoveSingle(2, 0, 9, 2)
        };

        EXPECT_TRUE(IsSameMove(actual, expected));
    }
}

TEST(HolePreservingMoveFinderTests, SwapRuns) {
    SpiderTableau tableau;

    auto& stacks = tableau.GetMutableStacks();

    SetStack(stacks[0], { D2H(), U3S() });
    SetStack(stacks[1], { D3H(), U4S() });
    SetStack(stacks[2], { D4H(), UTH(), U9H(), U8S(), U7S() });
    SetStack(stacks[3], { D4H(), U5S() });
    SetStack(stacks[4], { U6S(), U9S(), U8H(), U7H(), U6H() });
    // Stack 5 empty.
    SetStack(stacks[6], { D6H(), U7S() });
    SetStack(stacks[7], { D7H(), U6S(), });
    SetStack(stacks[8], { D8H(), U9S() });
    SetStack(stacks[9], { D9H(), UTS() });

/*
 0  1  2  3  4  5  6  7  8  9
:: :: :: :: 6S  - :: :: :: ::
3S 4S TH 5S 9S  - 7S 6S 9S TS
 -  - 9H  - 8H  -  -  -  -  -
 -  - 8S  - 7H  -  -  -  -  -
 -  - 7S  - 6H  -  -  -  -  -
*/

#ifdef _DEBUG
    PrintTableau(tableau);
#endif

    std::vector<MoveCombo> moves;
    int count = HolePreservingMoveFinder::AddSwapRuns(moves, tableau);

/*
  5{E0}2{3S2}, 2{9H7}4{6S4=E1} Trade-Holes
*/

#ifdef _DEBUG
    PrintMoves(tableau, moves);
#endif

    EXPECT_EQ(moves.size(), 1);
    EXPECT_EQ(count, 1);
    auto actual = moves[0].GetMoves();

    std::vector<MoveSingle> expected =
    {
        MoveSingle(2, 3, 5, 0),
        MoveSingle(4, 2, 2, 3),
        MoveSingle(5, 0, 4, 2)
    };

    EXPECT_TRUE(IsSameMove(actual, expected));
}

TEST(HolePreservingMoveFinderTests, MoveTwoSequentialRuns) {
    SpiderTableau tableau;

    auto& stacks = tableau.GetMutableStacks();

    SetStack(stacks[0], { D2H(), U3S() });
    SetStack(stacks[1], { D3H(), U4S() });
    SetStack(stacks[2], { D4H(), DTH(), U9H(), U8H() });
    SetStack(stacks[3], { D4H(), U5S() });
    SetStack(stacks[4], { U3S(), U2S(), U7H(), U6H(), U5S(), U4S(), U3S() });
    // Stack 5 empty.
    SetStack(stacks[6], { D6H(), U7S() });
    SetStack(stacks[7], { D7H(), U6S(), });
    SetStack(stacks[8], { D8H(), U9S() });
    SetStack(stacks[9], { D9H(), UTS() });

/*
 0  1  2  3  4  5  6  7  8  9
:: :: :: :: 3S  - :: :: :: ::
3S 4S :: 5S 2S  - 7S 6S 9S TS
 -  - 9H  - 7H  -  -  -  -  -
 -  - 8H  - 6H  -  -  -  -  -
 -  -  -  - 5S  -  -  -  -  -
 -  -  -  - 4S  -  -  -  -  -
 -  -  -  - 3S  -  -  -  -  -
*/

#ifdef _DEBUG
    PrintTableau(tableau);
#endif

    std::vector<MoveCombo> moves;
    int count = HolePreservingMoveFinder::AddMoveTwoRuns(moves, tableau);

/*
  2{9H8}4{7H6-5S3} Move-2-Seq-Runs
*/

#ifdef _DEBUG
    PrintMoves(tableau, moves);
#endif

    EXPECT_EQ(count, 1);
    EXPECT_EQ(moves.size(), 1);
    auto actual = moves[0].GetMoves();

    std::vector<MoveSingle> expected =
    {
        MoveSingle(4, 4, 5, 0),
        MoveSingle(4, 2, 2, 4),
        MoveSingle(5, 0, 2, 6)
    };

    EXPECT_TRUE(IsSameMove(actual, expected));
}


TEST(HolePreservingMoveFinderTests, RemoveMiddleRun) {
    SpiderTableau tableau;

    auto& stacks = tableau.GetMutableStacks();

    SetStack(stacks[0], { D2H(), U3S() });
    SetStack(stacks[1], { D3H(), U4S() });
    SetStack(stacks[2], { D4H(), DTH(), U9H(), U8H(), UJS(), UTS(), U7S(), U6S()});
    SetStack(stacks[3], { D4H(), U5S() });
    SetStack(stacks[4], { U3S(), U2S(), U7H(), UKH(), UQH() });
    // Stack 5 empty.
    SetStack(stacks[6], { D6H(), U7S() });
    SetStack(stacks[7], { D7H(), U6S(), });
    SetStack(stacks[8], { D8H(), U9S() });
    SetStack(stacks[9], { D9H(), UTS() });

/*
   0  1  2  3  4  5  6  7  8  9
0 :: :: :: :: 3S  - :: :: :: ::
1 3S 4S :: 5S 2S  - 7S 6S 9S TS
2  -  - 9H  - 7H  -  -  -  -  -
3  -  - 8H  - KH  -  -  -  -  -
4  -  - JS  - QH  -  -  -  -  -
5  -  - TS  -  -  -  -  -  -  -
6  -  - 7S  -  -  -  -  -  -  -
7  -  - 6S  -  -  -  -  -  -  -
*/

#ifdef _DEBUG
    PrintTableau(tableau);
#endif

    std::vector<MoveCombo> moves;
    int count = HolePreservingMoveFinder::AddRemoveMiddleRun(moves, tableau);

/*
   5{E0}2{7S6}, 4{KHQ}2{JST}, 2{9H8}5{7S6=E1} remove-middle-run
*/

#ifdef _DEBUG
    PrintMoves(tableau, moves);
#endif

    EXPECT_EQ(moves.size(), 1);
    EXPECT_EQ(count, 1);

    auto actual = moves[0].GetMoves();
    std::vector<MoveSingle> expected =
    {
        MoveSingle(2, 6, 5, 0),
        MoveSingle(2, 4, 4, 5),
        MoveSingle(5, 0, 2, 4)
    };
    EXPECT_TRUE(IsSameMove(actual, expected));
}


TEST(HolePreservingMoveFinderTests, InsertRunIntoStack) {
    SpiderTableau tableau;

    auto& stacks = tableau.GetMutableStacks();

    SetStack(stacks[0], { D2H(), U3S() });
    SetStack(stacks[1], { D3H(), U4S() });
    SetStack(stacks[2], { D4H(), DTH(), U9S(), U8S(), U4H(), U3H() });
    SetStack(stacks[3], { D4H(), U5S() });
    SetStack(stacks[4], { U3S(), U2S(), U7H(), U6H(), U5H() });
    // Stack 5 empty.
    SetStack(stacks[6], { D6H(), U7S() });
    SetStack(stacks[7], { D7H(), U6S(), });
    SetStack(stacks[8], { D8H(), U9S() });
    SetStack(stacks[9], { D9H(), UTS() });

/*
   0  1  2  3  4  5  6  7  8  9
0 :: :: :: :: 3S  - :: :: :: ::
1 3S 4S :: 5S 2S  - 7S 6S 9S TS
2  -  - 9S  - 7H  -  -  -  -  -
3  -  - 8S  - 6H  -  -  -  -  -
4  -  - 4H  - 5H  -  -  -  -  -
5  -  - 3H  -  -  -  -  -  -  -
*/

#ifdef _DEBUG
    PrintTableau(tableau);
#endif

    std::vector<MoveCombo> moves;
    int count = HolePreservingMoveFinder::AddInsertRun(moves, tableau);

/*
  5{E0}2{4H3}, 2{9S8}4{7H5}, 2{7H5}5{4H3=E1} insert-run
*/

#ifdef _DEBUG
    PrintMoves(tableau, moves);
#endif

    EXPECT_EQ(moves.size(), 1);
    EXPECT_EQ(count, 1);

    auto actual = moves[0].GetMoves();
    std::vector<MoveSingle> expected =
    {
        MoveSingle(2, 4, 5, 0),
        MoveSingle(4, 2, 2, 4),
        MoveSingle(5, 0, 2, 7)
    };
    EXPECT_TRUE(IsSameMove(actual, expected));
}