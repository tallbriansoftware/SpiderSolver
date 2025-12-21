#include "gtest/gtest.h"

#include "logic/MoveFinderSimple.h"

#include "spidersolvercore/strategy/Strategy.h"
#include "spidersolvercore/strategy/BoardScorer.h"
#include "spidersolvercore/model/MoveCombo.h"
#include "spidersolvercore/Model/SpiderTableau.h"
#include "spidersolvercore/utils/SpiderPrint.h"
#include "spidersolvercore/strategy/MoveChooser.h"

#include "Helpers/DeckHelper.h"

#include <vector>
#include <memory>


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

    void PrintMoves(const SpiderTableau& tableau, const std::vector<ScoredMove>& scoredMoves)
    {
        // output moves with scores
        for (auto& scoredMove : scoredMoves)
        {
            MoveCombo move = scoredMove.GetMove();
            std::string moveString = SpiderPrint::PrintBookMove(tableau, move, DoTurnCard::No);

            std::cout << scoredMove.GetScore();
            if (scoredMove.GetLocalScore() != -1.0)
                std::cout << "(" << scoredMove.GetLocalScore() << ")";
            std::cout << "   " << moveString << std::endl;
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


TEST(BoardScoringTests, FindMoveTest01) {
    SpiderTableau tableau;

    auto& stacks = tableau.GetMutableStacks();

    SetStack(stacks[0], { DAS(), DAS(), DAS(),
        UKH(), UQD() });
    SetStack(stacks[1], { DAS(), DAS(), DAS(), DAS(), DAS(),
        U4C(), U3C(), U2C(), UAC(),U3D(), U2D() });
    SetStack(stacks[2], { DAS(), DAS(), DAS(), DAS(), DAS(),
        UQD(), UJS() });
    // 3 is empty
    SetStack(stacks[4], { UQS(), UJS(), UTS(), U9S() });
    SetStack(stacks[5], { DAS(), DAS(), U9D() });
    SetStack(stacks[6], { DAS(),
        U8H(), U7S(), U6H(), U5H(), U4H(), U3S(), U2S(), UAH() });
    SetStack(stacks[7], { DAS(), DAS(), DAS(), DAS(),
        U4D(), UQH(), UJH(), UTH(), U9H(), U8H(), U7S(), U6H() });
    // 8 is empty
    SetStack(stacks[9], { DAS(), DAS(),
        U8C(), U7D(), U6C(), U5H(), U4S(), UTC(), U9C(), U8C(), U7D(), U6D(), U5S() });

    /*
            0  1  2  3  4  5  6  7  8  9
         0 :: :: ::  - QS :: :: ::  - ::
         1 :: :: ::  - JS :: 8H ::  - ::
         2 :: :: ::  - TS 9D 7S ::  - 8C
         3 KH :: ::  - 9S  - 6H ::  - 7D
         4 QD :: ::  -  -  - 5H 4D  - 6C
         5  - 4C QD  -  -  - 4H QH  - 5H
         6  - 3C JS  -  -  - 3S JH  - 4S
         7  - 2C  -  -  -  - 2S TH  - TC
         8  - AC  -  -  -  - AH 9H  - 9C
         9  - 3D  -  -  -  -  - 8H  - 8C
         10 - 2D  -  -  -  -  - 7S  - 7D
         11 -  -  -  -  -  -  - 6H  - 6D
         12 -  -  -  -  -  -  -  -  - 5S
    */

    Strategy strategy;

#ifdef _DEBUG
    PrintTableau(tableau);
    float boardScore = strategy.ComputeScore(tableau);
    std::cout << "Score = " << boardScore << std::endl;
#endif

    // Why do we not consider:
    //   moving Stack 1 4CA/3D2
    //   onto 5S in stack 9
    //   through the hole

    std::shared_ptr<SpiderTableau> sharedTableau = std::make_shared<SpiderTableau>(tableau);
    int depth = 6;

    MoveChooser moveChooser(sharedTableau, strategy, depth);
    auto move = moveChooser.ComputeBestMove();
    auto moves = moveChooser.GetAllChoices();
    PrintMoves(*sharedTableau, moves);
    size_t len = moves.size();
}

/*
* Move #85
* --count 1 --treeDepth 6 --suits 4 --randomSeed 36 -d
* 
        0  1  2  3  4  5  6  7  8  9
     0 :: :: ::  - QS :: :: ::  - ::
     1 :: :: ::  - JS :: 8H ::  - ::
     2 :: :: ::  - TS 9D 7S ::  - 8C
     3 KH :: ::  - 9S  - 6H ::  - 7D
     4 QD :: ::  -  -  - 5H 4D  - 6C
     5 JS 4C QD  -  -  - 4H QH  - 5H
     6 TC 3C  -  -  -  - 3D JH  -  -
     7 9C 2C  -  -  -  - 2D TH  -  -
     8 8C AC  -  -  -  -  - 9H  -  -
     9 7D  -  -  -  -  -  - 8H  -  -
     106D  -  -  -  -  -  - 7S  -  -
     11 -  -  -  -  -  -  - 6H  -  -
     12 -  -  -  -  -  -  - 5S  -  -
     13 -  -  -  -  -  -  - 4S  -  -
     14 -  -  -  -  -  -  - 3S  -  -
     15 -  -  -  -  -  -  - 2S  -  -
     16 -  -  -  -  -  -  - AH  -  -
     Deals-Left=4, Packs-Off=0
      Tur Hol Pck Del
       22   2   0   4
Board Score=165.3/541.2
167.4(166.3)   9{5H}<-1{4CA=?}
167.4(165.3)   6{3D2}<-7{AH}
167.4(165.3)   0{7D6}<-9{5H}
167.4(165.3)   0{7D6}<-7{5S2-AH} move-two-runs
*/