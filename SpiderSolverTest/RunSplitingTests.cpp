#include "gtest/gtest.h"

#include "logic/MoveFinderSimple.h"
#include "Logic/MoveFinderWithHole.h"

#include "spidersolvercore/strategy/Strategy.h"
#include "spidersolvercore/strategy/BoardScorer.h"
#include "spidersolvercore/model/MoveCombo.h"
#include "spidersolvercore/Model/SpiderTableau.h"
#include "spidersolvercore/utils/SpiderPrint.h"
#include "spidersolvercore/strategy/MoveChooser.h"

#include "Helpers/DeckHelper.h"
#include "Helpers/TestHelpers.h"

#include <vector>
#include <memory>


using namespace Cards;


TEST(HoleUsingMoveFinderTests, SplitRun) {
    SpiderTableau tableau;

    auto& stacks = tableau.GetMutableStacks();

    SetStack(stacks[3],
        { UKS(), UQS(), UJS(), UTS()});
    SetStack(stacks[4],
        { UJH(), UTH(), U9H(), U8H(), U7H() });

#ifdef _DEBUG
    PrintTableau(tableau);
#endif

    std::vector<MoveSingle> moves;
    MoveFinderSimple::AddRunSplittingMoves(moves, tableau);

    /*
        3<-4(2)
    */

#ifdef _DEBUG
    PrintMoves(tableau, moves);
#endif

    /*
       0  1  2  3  4  5  6  7  8  9
    0  -  -  - KS JH  -  -  -  -  -
    1  -  -  - QS TH  -  -  -  -  -
    2  -  -  - JS 9H  -  -  -  -  -
    3  -  -  - TS 8H  -  -  -  -  -
    4  -  -  -  - 7H  -  -  -  -  -
    */
    EXPECT_EQ(moves.size(), 1);
    auto actual = moves[0];
    MoveSingle expected(4, 2, 3, 4);
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
    // Breaking a suited run will lower the total score.
    EXPECT_LT(after, before);
}

TEST(HoleUsingMoveFinderTests, AddSplitRun) {
    SpiderTableau tableau;

    auto& stacks = tableau.GetMutableStacks();
    SetStack(stacks[1],
        { U9H(), U8H(), U7H(), U6D(), U5D() });
    SetStack(stacks[2],
        { UTH(), U9H(), U8H(), U7H(), U6D(), U5D() });
    SetStack(stacks[3],
        { UKS(), UQS(), UJS(), UTS() });
    SetStack(stacks[4],
        { U4S(), U3S(), UJH(), UTH(), U9H(), U8H(), U7H(), U6D(), U5D()});

#ifdef _DEBUG
    PrintTableau(tableau);
#endif

    std::vector<MoveCombo> moves;
    MoveFinderWithHole::AddSplitRun(moves, tableau);

/*
3{KST}<-4{9H7-6D5} split-run
*/

#ifdef _DEBUG
    PrintMoves(tableau, moves);
#endif

/*
   0  1  2  3  4  5  6  7  8  9
0  - 9H TH KS 4S  -  -  -  -  -
1  - 8H 9H QS 3S  -  -  -  -  -
2  - 7H 8H JS JH  -  -  -  -  -
3  - 6D 7H TS TH  -  -  -  -  -
4  - 5D 6D  - 9H  -  -  -  -  -
5  -  - 5D  - 8H  -  -  -  -  -
6  -  -  -  - 7H  -  -  -  -  -
7  -  -  -  - 6D  -  -  -  -  -
8  -  -  -  - 5D  -  -  -  -  -
*/
    EXPECT_EQ(moves.size(), 2);
    if (moves.size() == 0)
        return;

    auto actual0 = moves[0].GetMoves();
    std::vector<MoveSingle> expected0 =
    {
        MoveSingle(2, 4, 0, 0),
        MoveSingle(2, 1, 3, 4),
        MoveSingle(0, 0, 3, 7)
    };
    EXPECT_TRUE(IsSameMoves(actual0, expected0));

    auto actual1 = moves[1].GetMoves();
    std::vector<MoveSingle> expected1 =
    {
        MoveSingle(4, 7, 0, 0),
        MoveSingle(4, 4, 3, 4),
        MoveSingle(0, 0, 3, 7)
    };
    EXPECT_TRUE(IsSameMoves(actual1, expected1));

    Strategy strat;
    float before = strat.ComputeScore(tableau);

    auto move = moves[0];
    tableau.DoMove(move, DoTurnCard::Auto);

    float after = strat.ComputeScore(tableau);
    float diff = after - before;
#ifdef _DEBUG
    std::cout << "Score Difference: before=" << before << " - after=" << after << " = " << diff << std::endl;
#endif
    // Breaking a suited run will lower the total score.
    EXPECT_LT(after, before);
}

