#include "RunBoard.h"

#include "CommandLineArguments.h"

#include "Utils/ChronoTimer.h"

#include "spidersolvercore/model/Ancestry.h"
#include "spidersolvercore/model/TableauStats.h"
#include "spidersolvercore/model/SpiderTableauFactory.h"
#include "spidersolvercore/strategy/MoveChooser.h"
#include "spidersolvercore/strategy/ScoredMove.h"
#include "spidersolvercore/strategy/Strategy.h"
#include "spidersolvercore/strategy/StrategyUtil.h"
#include "spidersolvercore/utils/SpiderPrint.h"
#include "spidersolvercore/logic/MoveFinder.h"

#include <iostream>

void OutputBoard(const std::string& leadPadding, const SpiderTableau& tableau, const TableauStats& tStats)
{
    auto board = SpiderPrint::PrintTableau(tableau);
    for (auto line : board)
        std::cout << leadPadding << line << std::endl;

    std::cout << leadPadding << SpiderPrint::PrintTableauStatsHeaders() << std::endl;
    std::cout << leadPadding << SpiderPrint::PrintTableauStats(tStats) << std::endl;
    // std::cout << tableau.GetTableauString() << std::endl;
}


void OutputScoredMoves(const SpiderTableau& tableau, const std::vector<ScoredMove>& scoredMoves)
{
    // output moves with scores
    for (auto& scoredMove : scoredMoves)
    {
        std::string moveString = SpiderPrint::PrintBookMove(tableau, scoredMove.GetMove(), DoTurnCard::No);
        std::cout << scoredMove.GetScore();
        if (scoredMove.GetLocalScore() != -1.0)
            std::cout << "(" << scoredMove.GetLocalScore() << ")";
        std::cout << "   " << moveString << std::endl;
    }
}


BoardResult RunOneGame(
    const CommandLineArguments& args,
    int seed,
    Strategy& strategy,
    int depth)
{
    BoardResult result;

    int suits = args.GetSuits();

    auto sharedTableau = args.GetDealUp()
        ? SpiderTableauFactory::CreateAllUp(suits, seed)
        : SpiderTableauFactory::Create(suits, seed);

    bool printting = args.GetDisplay();
    MoveChooser moveChooser(sharedTableau, strategy, depth);
    std::shared_ptr<const SpiderTableau> tableauView = sharedTableau;
    int moveCount = 0;

    if (printting)
    {
        OutputBoard("     ", *tableauView, TableauStats(*tableauView));
    }
    MoveCombo move = moveChooser.ComputeBestMove();

    while(move.IsValid())
    {
        if (printting)
        {
            if (move.IsDeal())
            {
                std::cout << "=----------------- Deal -----------------" << std::endl;
            }
            else // normal move
            {
                auto otherMoves = moveChooser.GetAllChoices();
                if (!otherMoves.empty())
                {
                    OutputScoredMoves(*tableauView, otherMoves);

                    std::cout << "Move# " << moveCount << "\nMove: "
                        << SpiderPrint::PrintBookMove(*tableauView, move, DoTurnCard::Auto) << std::endl;
                }
                std::cout << "Evals = " << strategy.GetEvals() << std::endl;
            }
        }

        moveCount += move.Count();
        moveChooser.CommitMove(move);

        auto currentScore = strategy.ComputeScore(*tableauView);

        if (printting)
        {
            OutputBoard("     ", *tableauView, TableauStats(*tableauView));
            std::cout << "Board Score=" << currentScore << "/" << strategy.MaxScore() << std::endl;
        }

        // collect moves for the next time around
        move = moveChooser.ComputeBestMove();
    } while (move.IsValid());

    result.score = strategy.ComputeScore(*tableauView);
    result.searchDepth = depth;
    result.won = tableauView->IsWon();
    result.moveCount = moveCount;
    result.evals = strategy.GetEvals();
    return result;
}


BoardResult RunOneGameOuter(
    const CommandLineArguments& args,
    int seed,
    Strategy& strategy,
    int depth)
{
    ChronoTimer timer;
    timer.Start();

    BoardResult result = RunOneGame(args, seed, strategy, depth);

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
