#include "RunBoard.h"

#include "CommandLineArguments.h"

#include "Utils/ChronoTimer.h"

#include "spidersolvercore/model/Ancestry.h"
#include "spidersolvercore/model/TableauStats.h"
#include "spidersolvercore/model/SpiderTableauFactory.h"
#include "spidersolvercore/strategy/ScoredMove.h"
#include "spidersolvercore/strategy/Strategy.h"
#include "spidersolvercore/strategy/StrategyUtil.h"
#include "spidersolvercore/utils/SpiderPrint.h"

#include <iostream>

void OutputBoard(const std::string& leadPadding, const SpiderTableau& tableau, const TableauStats& tStats)
{
    auto board = SpiderPrint::PrintTableau(tableau);
    for (auto line : board)
        std::cout << leadPadding << line << std::endl;

    std::cout << leadPadding << SpiderPrint::PrintTableauStatsHeaders() << std::endl;
    std::cout << leadPadding << SpiderPrint::PrintTableauStats(tStats) << std::endl;
    std::cout << tableau.GetTableauString() << std::endl;
}


void OutputScoredMoves(const SpiderTableau& tableau, const std::vector<ScoredMove>& scoredMoves)
{
    // output moves with scores
    for (auto& scoredMove : scoredMoves)
    {
        std::string moveString = SpiderPrint::PrintBookMove(tableau, scoredMove.move, DoTurnCard::No);
        std::cout << scoredMove.localScore << "   " << moveString << std::endl;
    }
}


BoardResult RunBoardInner(const CommandLineArguments& args, int seed, Strategy& strategy)
{
    BoardResult result;

    auto sPtr_tableau = args.GetDealUp()
        ? SpiderTableauFactory::CreateAllUp(2, seed)
        : SpiderTableauFactory::Create(2, seed);

    bool printting = args.GetDisplay();

    SpiderTableau tableau = *sPtr_tableau;
    Ancestry ancestry(tableau);

    int moveCount = 0;

    while (true)
    {
        auto currentScore = strategy.ComputeScore(tableau);
        if (printting)
        {
            OutputBoard("     ", tableau, TableauStats(tableau));
            std::cout << "Board Score=" << currentScore << "/" << strategy.MaxScore() << std::endl;
        }

        auto scoredMoves = strategy.FindScoredMoves(tableau, ancestry);
        if (!scoredMoves.empty())
        {
            StrategyUtil::SortLocalMoves(scoredMoves);
            if (printting)
            {
                OutputScoredMoves(tableau, scoredMoves);
            }

            float score = scoredMoves[0].localScore;
            MoveCombo move = scoredMoves[0].move;

            moveCount += move.Count();

            if (printting)
            {
                std::cout << "Move # " << moveCount << ": " << score << " = "
                    << SpiderPrint::PrintBookMove(tableau, move, DoTurnCard::Auto) << std::endl;
            }

            tableau.DoMove(move, DoTurnCard::Auto);
        }

        if (scoredMoves.empty())
        {
            if (!tableau.CanDeal())
                break;
            if (!tableau.DealTurn())
                throw std::exception("can't deal as expected");

            if (printting)
                std::cout << "=----------------- Deal -----------------" << std::endl;

            moveCount += 1;
        }
        ancestry.AddTableau(tableau);
    }

    result.score = strategy.ComputeScore(tableau);
    result.won = tableau.IsWon();
    result.moveCount = moveCount;
    result.evals = strategy.GetEvals();
    return result;
}


BoardResult RunOneBoard(const CommandLineArguments& args, int seed, Strategy& strategy)
{
    ChronoTimer timer;

    timer.Start();
    BoardResult result = RunBoardInner(args, seed, strategy);
    result.usecs = timer.ReadMicroseconds();

    // Some Error Checking
    const double MaxTableauScore = strategy.MaxScore();
    bool scoreWin = (result.score >= MaxTableauScore);
    if (scoreWin != result.won)
    {
        std::cerr << "Winning Score of " << result.score << " is incorrect.  Defined winning score is " << MaxTableauScore << std::endl;
        throw std::exception("wrong");
    }

    return result;
}
