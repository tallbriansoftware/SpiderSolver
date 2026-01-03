#include "gtest/gtest.h"

#include "logic/MoveFinderSimple.h"

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
