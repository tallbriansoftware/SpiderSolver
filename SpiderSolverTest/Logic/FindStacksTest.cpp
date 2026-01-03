#include "gtest/gtest.h"

#include "spidersolvercore/logic/FindStacks.h"
#include "spidersolvercore/model/MoveCombo.h"
#include "spidersolvercore/model/SpiderStack.h"
#include "spidersolvercore/utils/SpiderPrint.h"

#include "Helpers/DeckHelper.h"
#include "Helpers/TestHelpers.h"

#include <iostream>

using namespace Cards;

TEST(HolePreservingMoveFinderTests, RunPatternTest1) {
    SpiderStack stack;
    stack.SetIsColumn();

    SetStack(stack, { D2H(), U7S(), U6S(), U5H(), U4H() });
    EXPECT_TRUE(FindStacks::RunPattern(stack, 2, Exactly::EqualOrGreator, Sequential::Yes));
}

TEST(HolePreservingMoveFinderTests, RunPatternTest2) {
    {
        SpiderStack stack;
        stack.SetIsColumn();
        SetStack(stack, { D2H(), U7S() });
        EXPECT_TRUE(FindStacks::RunPattern(stack, 1, Exactly::EqualOrGreator, Sequential::Yes));
        EXPECT_FALSE(FindStacks::RunPattern(stack, 1, Exactly::Equal, Sequential::Yes));
    }
    {
        SpiderStack stack;
        stack.SetIsColumn();
        SetStack(stack, { D2H(), U7S(), U6S(), U5H(), U4H(), U3S(), U2S() });
        EXPECT_TRUE(FindStacks::RunPattern(stack, 3, Exactly::EqualOrGreator, Sequential::Yes));
        EXPECT_FALSE(FindStacks::RunPattern(stack, 3, Exactly::Equal, Sequential::Yes));
    }
    {
        SpiderStack stack;
        stack.SetIsColumn();
        SetStack(stack, { U2H(), U7S(), U6S(), U5H(), U4H(), U3S(), U2S() });
        EXPECT_TRUE(FindStacks::RunPattern(stack, 3, Exactly::EqualOrGreator, Sequential::Yes));
        EXPECT_FALSE(FindStacks::RunPattern(stack, 3, Exactly::Equal, Sequential::Yes));
    }
    {
        SpiderStack stack;
        stack.SetIsColumn();
        SetStack(stack, {        U7S(), U6S(), U5H(), U4H(), U3S(), U2S() });
        EXPECT_TRUE(FindStacks::RunPattern(stack, 3, Exactly::EqualOrGreator, Sequential::Yes));
        EXPECT_TRUE(FindStacks::RunPattern(stack, 3, Exactly::Equal, Sequential::Yes));
    }
}

TEST(HolePreservingMoveFinderTests, RunPatternTest3) {
    SpiderStack stack;
    stack.SetIsColumn();

    SetStack(stack, { D2H(), U7S(), U6H(), U6S() });
    EXPECT_FALSE(FindStacks::RunPattern(stack, 3, Exactly::EqualOrGreator, Sequential::Yes));
}

