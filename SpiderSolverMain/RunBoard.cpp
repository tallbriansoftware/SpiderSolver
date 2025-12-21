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

void OutputScoredMoves(
    const SpiderTableau& tableau,
    const std::vector<ScoredMove>& scoredMoves)
{
    // output moves with scores
    for (auto& scoredMove : scoredMoves)
    {
         std::string moveString = SpiderPrint::PrintBookMove(
         tableau,
         scoredMove.GetMove(),
         DoTurnCard::No);

        std::cout << scoredMove.GetScore();
        if (scoredMove.GetLocalScore() != -1.0)
            std::cout << "(" << scoredMove.GetLocalScore() << ")";
        std::cout << "   " << moveString << std::endl;
    }
}

void OutputSkippedMoves(
    const SpiderTableau& tableau,
    const std::vector<MoveCombo>& skippedMoves)
{
    for (auto skipMove : skippedMoves)
    {
        std::string moveString = SpiderPrint::PrintBookMove(tableau, skipMove, DoTurnCard::No);
        std::cout << "(*) " << moveString << std::endl;
    }
}

void Pr_ShowMoves(
    const CommandLineArguments& args,
    const MoveCombo& move,
    const SpiderTableau& tableauView,
    const MoveChooser& moveChooser
)
{
    bool printting = args.GetDisplay();
    if (printting)
    {
        if (!move.IsDeal())
        {
            const std::vector<ScoredMove>& otherMoves = moveChooser.GetAllChoices();
            if (!otherMoves.empty())
            {
                OutputScoredMoves(tableauView, otherMoves);
            }

            const std::vector<MoveCombo>& skippedMoves = moveChooser.GetDisregardedChoices();
            if (!skippedMoves.empty())
            {
                OutputSkippedMoves(tableauView, skippedMoves);
            }
        }
    }
}

void Pr_ShowTurnStats(
    const CommandLineArguments& args,
    const MoveCombo& move,
    const SpiderTableau& tableauView,
    const MoveChooser& moveChooser,
    const Strategy& strategy)
{
    bool printting = args.GetDisplay();
    if (printting)
    {
        std::cout << "Move# " << moveChooser.GetMoveNumber() << ": ";

        if (move.IsDeal())
            std::cout << "--- Deal --- " << std::endl;
        else
            std::cout << SpiderPrint::PrintBookMove(tableauView, move, DoTurnCard::Auto) << std::endl;

        std::cout << "Evals = " << strategy.GetEvals() << std::endl;
    }
}

void Pr_OutputBoard(
    const CommandLineArguments& args,
    const SpiderTableau& tableauView,
    const Strategy& strategy)
{
    bool printting = args.GetDisplay();
    if (printting)
    {
        auto currentScore = strategy.ComputeScore(tableauView);
        OutputBoard("     ", tableauView, TableauStats(tableauView));
        std::cout << "Board Score=" << currentScore << "/" << strategy.MaxScore() << std::endl;
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

    MoveChooser moveChooser(sharedTableau, strategy, depth);
    std::shared_ptr<const SpiderTableau> tableauView = sharedTableau;

    Pr_OutputBoard(args, *tableauView, strategy);
    MoveCombo move = moveChooser.ComputeBestMove();

    while(move.IsValid())
    {
        Pr_ShowMoves(args, move, *tableauView, moveChooser);
        Pr_ShowTurnStats(args, move, *tableauView, moveChooser, strategy);

        moveChooser.CommitMove(move);

        Pr_OutputBoard(args, *tableauView, strategy);
        move = moveChooser.ComputeBestMove();
    }

    result.score = strategy.ComputeScore(*tableauView);
    result.searchDepth = depth;
    result.won = tableauView->IsWon();
    result.moveCount = moveChooser.GetMoveNumber();
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
