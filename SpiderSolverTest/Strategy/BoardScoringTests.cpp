#include "gtest/gtest.h"

#include "logic/MoveFinderSimple.h"

#include "spidersolvercore/strategy/Strategy.h"
#include "spidersolvercore/strategy/BoardScorer.h"
#include "spidersolvercore/model/MoveCombo.h"
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
        if (expected.FromStack() != actual.FromStack())
            return false;
        if (expected.FromIndex() != actual.FromIndex())
            return false;
        if (expected.DestStack() != actual.DestStack())
            return false;
        if (expected.DestIndex() != actual.DestIndex())
            return false;
        return true;
    }
}


TEST(BoardScoringTests, CountingHoles) {
    SpiderTableau tableau;

    auto& stacks = tableau.GetMutableStacks();

    SetStack(stacks[1], { DAS(), DAS(), DAS(), DAS(), DAS(),
        U8S(), U7H(), U6H(), U5S(), U4H(), U3H(), U2H(), UAH() });
    SetStack(stacks[2], { DAS(), DAS(), DAS(), DAS(), DAS(),
        UKS(), UQS() });
    SetStack(stacks[3], { DAS(), DAS(), DAS(), DAS(), DAS(),
        U2S(), UAS() });
    SetStack(stacks[4], { DAS(), UTS(), U9H() });
    SetStack(stacks[5], { DAS(), DAS(), DAS(), DAS(),
        U7H(), U6H(), U5S(), U4H() });
    SetStack(stacks[6], { DAS(), UTS() });

    SetStack(stacks[8], { DAS(), DAS(), DAS(), DAS(),
        UKH(), UQS() });
    SetStack(stacks[9], { DAS(), DAS(), DAS(), DAS(),
        UTS(), U9H(), U8H(), U7S() });

    /* 0  1  2  3  4  5  6  7  8  9
    0  - :: :: :: :: :: ::  - :: ::
    1  - :: :: :: TS :: TS  - :: ::
    2  - :: :: :: 9H ::  -  - :: ::
    3  - :: :: ::  - ::  -  - :: ::
    4  - :: :: ::  - 7H  -  - KH TS
    5  - 8S KS 2S  - 6H  -  - QS 9H
    6  - 7H QS AS  - 5S  -  -  - 8H
    7  - 6H  -  -  - 4H  -  -  - 7S
    8  - 5S  -  -  -  -  -  -  -  -
    9  - 4H  -  -  -  -  -  -  -  -
    10 - 3H  -  -  -  -  -  -  -  -
    11 - 2H  -  -  -  -  -  -  -  -
    12 - AH  -  -  -  -  -  -  -  -*/

#ifdef _DEBUG
    PrintTableau(tableau);
#endif

    BoardScorer scorer;
    MoveCombo move1(MoveSingle(1, 9, 0, 0));
    std::vector<MoveSingle> moveVec{
        MoveSingle(9, 7, 0, 0),
        MoveSingle(9, 5, 6, 2),
        MoveSingle(0, 0, 6, 4) };
    MoveCombo move2(moveVec);

    float s1 = 0.0;
    float s2 = 0.0;
    {
        SpiderTableau::SavePoint save(tableau);
        tableau.DoMove(move1, DoTurnCard::No);
        s1 = scorer.ComputeScore(tableau);
    }
    {
        SpiderTableau::SavePoint save(tableau);
        tableau.DoMove(move2, DoTurnCard::No);
        s2 = scorer.ComputeScore(tableau);
    }
    // Move1 filled one of two holes.
    // Move2 leaves both holes open.
    EXPECT_GT(s2, s1);
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
    MoveFinderSimple::AddColorUpMoves(moves, tableau);
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
        { UKS(), UQS(), UJS(), UTS(), U9S(), U8S(), U7S(), U6S(), U5S() });
    SetStack(stacks[4],
        { U6S(), U5S(), U4S() ,U3S(), U2S(), UAS() });

#ifdef _DEBUG
    PrintTableau(tableau);
#endif

    std::vector<MoveSingle> moves;
    MoveFinderSimple::AddColorUpMoves(moves, tableau);
    EXPECT_EQ(moves.size(), 1);

/*
    3<-4(2)
*/
#ifdef _DEBUG
    PrintMoves(tableau, moves);
#endif

/*
   0  1  2  3  4  5  6  7  8  9
0  -  -  - KS 6S  -  -  -  -  -
1  -  -  - QS 5S  -  -  -  -  -
2  -  -  - JS 4S  -  -  -  -  -
3  -  -  - TS 3S  -  -  -  -  -
4  -  -  - 9S 2S  -  -  -  -  -
5  -  -  - 8S  -  -  -  -  -  -
6  -  -  - 7S  -  -  -  -  -  -
7  -  -  - 6S  -  -  -  -  -  -
8  -  -  - 5S  -  -  -  -  -  -
*/

    EXPECT_EQ(moves.size(), 1);
    auto actual = moves[0];
    MoveSingle expected(4, 2, 3, 9);
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
        { UKS(), UQS(), UJS(), UTS(), U9S(), U8S(), U7S(), U6S(), U5S(), U4S(), U3S() });
    SetStack(stacks[4],
        { UQS(), UJS(), UTS(), U9S(), U8S(), U7S(), U6S(), U5S(), U4S(), U3S(), U2S()});

#ifdef _DEBUG
    PrintTableau(tableau);
#endif

    std::vector<MoveSingle> moves;
    MoveFinderSimple::AddColorUpMoves(moves, tableau);
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
11 -  -  -  -  -  -  -  -  -  -
*/

    EXPECT_EQ(moves.size(), 1);
    auto actual = moves[0];
    MoveSingle expected(4, 10, 3, 11);
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
    MoveFinderSimple::AddColorUpMoves(moves, tableau);
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

