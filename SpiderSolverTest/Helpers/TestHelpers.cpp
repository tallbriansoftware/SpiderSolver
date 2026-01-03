#include "Helpers/TestHelpers.h"

#include "spidersolvercore/model/SpiderTableau.h"
#include "spidersolvercore/utils/SpiderPrint.h"
#include "spidersolvercore/model/MoveSingle.h"
#include "spidersolvercore/model/MoveCombo.h"
#include "spidersolvercore/strategy/ScoredMove.h"

#include <iostream>

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

bool IsSameMove(const MoveSingle& actual, const MoveSingle& expected)
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

bool IsSameMoves(const std::vector<MoveSingle>& actual, const std::vector<MoveSingle>& expected)
{
    if (actual.size() != expected.size())
        return false;

    for (int i = 0; i < actual.size(); i++)
    {
        if (!IsSameMove(expected[i], actual[i]))
            return false;
    }
    return true;
}
