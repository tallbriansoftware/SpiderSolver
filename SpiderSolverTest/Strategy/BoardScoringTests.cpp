#include "gtest/gtest.h"

#include "spidersolvercore/strategy/Strategy.h"
#include "spidersolvercore/logic/MoveFinder.h"
#include "spidersolvercore/Model/SpiderTableau.h"
#include "spidersolvercore/utils/SpiderPrint.h"

#include "Helpers/DeckHelper.h"
using namespace Cards;

namespace {
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

    void PrintMoves(const SpiderTableau& tableau, const std::vector<MoveSingle>& moves)
    {
        for (auto& move : moves)
        {
            std::string moveString = SpiderPrint::PrintPlainMove(move);
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
}


TEST(BoardScoringTests, twoLongs) {
    SpiderTableau tableau;

    auto& stacks = tableau.GetMutableStacks();

    SetStack(stacks[3],
        { UJS(), UTS(), U9S(), U8S(), U7S() });
    SetStack(stacks[4],
        { U8S(), U7S(), U6S(), U5S(), U4S() });
/*
   0  1  2  3  4  5  6  7  8  9
0  -  -  - JS 8S  -  -  -  -  -
1  -  -  - TS 7S  -  -  -  -  -
2  -  -  - 9S 6S  -  -  -  -  -
3  -  -  - 8S 5S  -  -  -  -  -
4  -  -  - 7S 4S  -  -  -  -  -
*/
#ifdef _DEBUG
    PrintTableau(tableau);
#endif

    std::vector<MoveSingle> moves;
    MoveFinder::AddColorUpMoves(moves, tableau);
    EXPECT_EQ(moves.size(), 1);

/*
    3<-4(2)
*/
#ifdef _DEBUG
    PrintMoves(tableau, moves);
#endif
    EXPECT_EQ(moves.size(), 1);
    auto actual = moves[0];
    MoveSingle expected(4, 2, 3, 5);
    EXPECT_TRUE(IsSameMove(actual, expected));

    Strategy strat;
    float before = strat.ComputeScore(tableau);

    tableau.DoMove(actual, DoTurnCard::Auto);

    float after = strat.ComputeScore(tableau);
    float diff = after - before;
#ifdef _DEBUG
    std::cout << "Score Difference: before=" << before << " - after=" << after << " = " << diff << std::endl;
#endif
    EXPECT_GT(after, before);
}

TEST(BoardScoringTests, VeryLongAndShort) {
    SpiderTableau tableau;

    auto& stacks = tableau.GetMutableStacks();

    SetStack(stacks[3],
        { UKS(), UQS(), UJS(), UTS(), U9S(), U8S(), U7S(), U6S(), U5S(), U4S(), U3S(), U2S()});
    SetStack(stacks[4],
        { U3S(), U2S(), UAS() });

#ifdef _DEBUG
    PrintTableau(tableau);
#endif

    std::vector<MoveSingle> moves;
    MoveFinder::AddColorUpMoves(moves, tableau);
    EXPECT_EQ(moves.size(), 1);

/*
    3<-4(2)
*/
#ifdef _DEBUG
    PrintMoves(tableau, moves);
#endif

/*
   0  1  2  3  4  5  6  7  8  9
0  -  -  - KS 3S  -  -  -  -  -
1  -  -  - QS 2S  -  -  -  -  -
2  -  -  - JS AS  -  -  -  -  -
3  -  -  - TS  -  -  -  -  -  -
4  -  -  - 9S  -  -  -  -  -  -
5  -  -  - 8S  -  -  -  -  -  -
6  -  -  - 7S  -  -  -  -  -  -
7  -  -  - 6S  -  -  -  -  -  -
8  -  -  - 5S  -  -  -  -  -  -
9  -  -  - 4S  -  -  -  -  -  -
10 -  -  - 3S  -  -  -  -  -  -
11 -  -  - 2S  -  -  -  -  -  -
*/

    EXPECT_EQ(moves.size(), 1);
    auto actual = moves[0];
    MoveSingle expected(4, 2, 3, 12);
    EXPECT_TRUE(IsSameMove(actual, expected));

    Strategy strat;
    float before = strat.ComputeScore(tableau);

    auto move = moves[0];
    tableau.DoMove(move, DoTurnCard::Auto);

    float after = strat.ComputeScore(tableau);
    float diff = after - before;
#ifdef _DEBUG
    std::cout << "Score Difference: before=" << before << " - after=" << after << " = " << diff << std::endl;
#endif
    EXPECT_GT(after, before);
}


TEST(BoardScoringTests, TwoVeryLong) {
    SpiderTableau tableau;

    auto& stacks = tableau.GetMutableStacks();

    SetStack(stacks[3],
        { UKS(), UQS(), UJS(), UTS(), U9S(), U8S(), U7S(), U6S(), U5S(), U4S(), U3S(), U2S() });
    SetStack(stacks[4],
        { UQS(), UJS(), UTS(), U9S(), U8S(), U7S(), U6S(), U5S(), U4S(), U3S(), U2S(), UAS()});

#ifdef _DEBUG
    PrintTableau(tableau);
#endif

    std::vector<MoveSingle> moves;
    MoveFinder::AddColorUpMoves(moves, tableau);
    EXPECT_EQ(moves.size(), 1);

/*
    3<-4(11)
*/

#ifdef _DEBUG
    PrintMoves(tableau, moves);
#endif

/*
   0  1  2  3  4  5  6  7  8  9
0  -  -  - KS QS  -  -  -  -  -
1  -  -  - QS JS  -  -  -  -  -
2  -  -  - JS TS  -  -  -  -  -
3  -  -  - TS 9S  -  -  -  -  -
4  -  -  - 9S 8S  -  -  -  -  -
5  -  -  - 8S 7S  -  -  -  -  -
6  -  -  - 7S 6S  -  -  -  -  -
7  -  -  - 6S 5S  -  -  -  -  -
8  -  -  - 5S 4S  -  -  -  -  -
9  -  -  - 4S 3S  -  -  -  -  -
10 -  -  - 3S 2S  -  -  -  -  -
11 -  -  - 2S AS  -  -  -  -  -
*/

    EXPECT_EQ(moves.size(), 1);
    auto actual = moves[0];
    MoveSingle expected(4, 11, 3, 12);
    EXPECT_TRUE(IsSameMove(actual, expected));

    Strategy strat;
    float before = strat.ComputeScore(tableau);

    auto move = moves[0];
    tableau.DoMove(move, DoTurnCard::Auto);

    float after = strat.ComputeScore(tableau);
    float diff = after - before;
#ifdef _DEBUG
    std::cout << "Score Difference: before=" << before << " - after=" << after << " = " << diff << std::endl;
#endif
    EXPECT_GT(after, before);
}

TEST(BoardScoringTests, ShortAndLong) {
    SpiderTableau tableau;

    auto& stacks = tableau.GetMutableStacks();

    SetStack(stacks[3],
        { UKS(), UQS() });
    SetStack(stacks[4],
        { UQS(), UJS(), UTS(), U9S(), U8S(), U7S(), U6S(), U5S(), U4S(), U3S(), U2S(), UAS() });

#ifdef _DEBUG
    PrintTableau(tableau);
#endif

    std::vector<MoveSingle> moves;
    MoveFinder::AddColorUpMoves(moves, tableau);
    EXPECT_EQ(moves.size(), 1);

/*
    3<-4(1)
*/

#ifdef _DEBUG
    PrintMoves(tableau, moves);
#endif

/*
   0  1  2  3  4  5  6  7  8  9
0  -  -  - KS QS  -  -  -  -  -
1  -  -  - QS JS  -  -  -  -  -
2  -  -  -  - TS  -  -  -  -  -
3  -  -  -  - 9S  -  -  -  -  -
4  -  -  -  - 8S  -  -  -  -  -
5  -  -  -  - 7S  -  -  -  -  -
6  -  -  -  - 6S  -  -  -  -  -
7  -  -  -  - 5S  -  -  -  -  -
8  -  -  -  - 4S  -  -  -  -  -
9  -  -  -  - 3S  -  -  -  -  -
10 -  -  -  - 2S  -  -  -  -  -
11 -  -  -  - AS  -  -  -  -  -
*/

    EXPECT_EQ(moves.size(), 1);
    auto actual = moves[0];
    MoveSingle expected(4, 1, 3, 2);
    EXPECT_TRUE(IsSameMove(actual, expected));

    Strategy strat;
    float before = strat.ComputeScore(tableau);

    auto move = moves[0];
    tableau.DoMove(move, DoTurnCard::Auto);

    float after = strat.ComputeScore(tableau);
    float diff = after - before;
#ifdef _DEBUG
    std::cout << "Score Difference: before=" << before << " - after=" << after << " = " << diff << std::endl;
#endif
    EXPECT_GT(after, before);
}

