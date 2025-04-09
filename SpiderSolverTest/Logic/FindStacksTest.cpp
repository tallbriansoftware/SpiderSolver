#include "gtest/gtest.h"

#include "spidersolvercore/logic/FindStacks.h"
#include "spidersolvercore/model/MoveCombo.h"
#include "spidersolvercore/model/SpiderStack.h"
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
}

using namespace Cards;

TEST(HolePreservingMoveFinderTests, RunPatternTest1) {
    SpiderStack stack;
    stack.SetIsColumn();

    SetStack(stack, { D2H(), U7S(), U6S(), U5H(), U4H() });
    EXPECT_TRUE(FindStacks::RunPattern(stack, 2, Exactly::No, Sequential::Yes));
}

TEST(HolePreservingMoveFinderTests, RunPatternTest2) {
    {
        SpiderStack stack;
        stack.SetIsColumn();
        SetStack(stack, { D2H(), U7S(), U6S(), U5H(), U4H(), U3S(), U2S() });
        EXPECT_TRUE(FindStacks::RunPattern(stack, 3, Exactly::No, Sequential::Yes));
        EXPECT_FALSE(FindStacks::RunPattern(stack, 3, Exactly::Yes, Sequential::Yes));
    }
    {
        SpiderStack stack;
        stack.SetIsColumn();
        SetStack(stack, { U2H(), U7S(), U6S(), U5H(), U4H(), U3S(), U2S() });
        EXPECT_TRUE(FindStacks::RunPattern(stack, 3, Exactly::No, Sequential::Yes));
        EXPECT_FALSE(FindStacks::RunPattern(stack, 3, Exactly::Yes, Sequential::Yes));
    }
    {
        SpiderStack stack;
        stack.SetIsColumn();
        SetStack(stack, {        U7S(), U6S(), U5H(), U4H(), U3S(), U2S() });
        EXPECT_TRUE(FindStacks::RunPattern(stack, 3, Exactly::No, Sequential::Yes));
        EXPECT_TRUE(FindStacks::RunPattern(stack, 3, Exactly::Yes, Sequential::Yes));
    }
}

TEST(HolePreservingMoveFinderTests, RunPatternTest3) {
    SpiderStack stack;
    stack.SetIsColumn();

    SetStack(stack, { D2H(), U7S(), U6H(), U6S() });
    EXPECT_FALSE(FindStacks::RunPattern(stack, 3, Exactly::No, Sequential::Yes));
}

