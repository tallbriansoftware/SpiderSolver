#include "CommandLineArguments.h"
#include "MyCsv.h"
#include "RunBoard.h"
#include "SeriesTotal.h"

#include "Utils/ChronoTimer.h"
#include "Utils/CsvTable.h"
#include "Utils/DateTime.h"

#include "spidersolvercore/strategy/BoardScorer.h"
#include "spidersolvercore/strategy/Strategy.h"

#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>


void  CollectTotals(const BoardResult& result, SeriesTotal& totals)
{
    totals.time += result.usecs;
    if (result.won)
        totals.wins += 1;
    else
        totals.losingScore += result.score;
}

std::vector<std::unique_ptr<Strategy>> GetStrategies(const CommandLineArguments& args)
{
    std::vector<std::unique_ptr<Strategy>> strats;
    auto strat = std::make_unique<Strategy>();
    strats.emplace_back(move(strat));
    return strats;

// ===============================================

    // Just return the above default strategy until
    // the command line processing of parameters is ready.

    for (int i = 0; i < 5; i++)
    {
        for (int k = 0; k < 5; k++)
        {
            BoardScorer boardScorer;
            boardScorer.SetHolesTerm((float)(6 + i * 1.0));
            boardScorer.SetTurnedCardsTerm((float)(3.0 + k * 0.2));

            auto strat = std::make_unique<Strategy>(boardScorer);
            strats.emplace_back(move(strat));
        }
    }

    return strats;
}

void AddFloatColumnsToCsv(CsvTable& csv, std::vector<std::string>& names)
{
    for (auto& name : names)
    {
        CsvTable::Column col = { name, CsvTable::typeFloat };
        csv.AddColumn(name, CsvTable::typeFloat);
    }
}

SeriesTotal RunASeriesOfGames(const CommandLineArguments& args, CsvTable& csv)
{
    ChronoTimer timer;
    std::string currentDate = GetCurrentDate();

    int start = args.GetRandomSeed();
    int count = args.GetCount();
    int depth = args.GetTreeDepth();

    auto strategyList = GetStrategies(args);
    int stratCount = (int)strategyList.size();
    SeriesTotal total = { 0, 0, 0, 0.0 };
    auto termNames = strategyList[0]->GetModifiedTermNames();
    AddFloatColumnsToCsv(csv, termNames);

    for (auto& stratPtr : strategyList)
    {
        Strategy& strategy = *stratPtr.get();
        auto termValues = strategy.GetModifiedTerms();

        int suits = args.GetSuits();
        bool dealUp = args.GetDealUp();

        int totalCount = count * stratCount;
        for (int seed = start; seed < start + count; ++seed)
        {
            csv.StartRow();
            csv.AddValue(MyCsv::dateHeader, currentDate);
            csv.AddValue(MyCsv::seedHeader, seed);
            csv.AddValue(MyCsv::depthHeader, depth);
            csv.AddValue(MyCsv::numSuitsHeader, suits);
            csv.AddValue(MyCsv::dealtUpHeader, dealUp);

            strategy.ClearEvals();
            BoardResult result = RunOneGameOuter(args, seed, strategy, depth);

            csv.AddValue(MyCsv::movesHeader, result.moveCount);
            csv.AddValue(MyCsv::scoreHeader, result.score);
            csv.AddValue(MyCsv::evalsHeader, result.evals);
            csv.AddValue(MyCsv::nanosecsHeader, result.usecs);
            csv.AddValue(MyCsv::timedOutHeader, result.timedOut);
            csv.AddValue(MyCsv::winHeader, result.won);


            for (int i = 0; i < (int)termNames.size(); i++)
                csv.AddValue(termNames[i], termValues[i]);

            CollectTotals(result, total);
            total.completed += 1;
            std::cerr << (total.completed * 100) / totalCount << "% complete(" << total.completed
                << " of " << totalCount << ")   " << total.wins << " Wins("
                << (total.wins * 100) / total.completed << "%) " << " Last completed seed: "
                << seed << "               \r";

            csv.EndRow();
        }
        std::cerr << std::endl;

    }
    return total;
}

void OutputSeriesTotals(int count, const SeriesTotal& totals)
{
    std::cerr
        << totals.wins << " Wins " << totals.completed << " Total.  "
        << (totals.wins * 100) / totals.completed << "% win rate."
        << "  Avg losing score " << totals.losingScore / ((int64_t)totals.completed - totals.wins)
        << "  Total Time " << totals.time / 1000000.0
        << " (" << totals.time / (float)(totals.completed * 1000000) << " secs avg)."
        << std::endl;
}

int main(int argc, char* argv[])
{
    auto args = CommandLineArguments(argc, argv);
    if (!args.Parse())
    {
        args.Usage();
        std::exit(1);
    }

    std::vector<CsvTable::Column> columns = {
        {MyCsv::dateHeader, CsvTable::typeString},
        {MyCsv::seedHeader, CsvTable::typeInt},
        {MyCsv::numSuitsHeader, CsvTable::typeInt},
        {MyCsv::dealtUpHeader, CsvTable::typeBool},

        {MyCsv::movesHeader, CsvTable::typeInt},
        {MyCsv::scoreHeader, CsvTable::typeFloat},
        {MyCsv::evalsHeader, CsvTable::typeInt},
        {MyCsv::nanosecsHeader, CsvTable::typeInt64},
        {MyCsv::depthHeader, CsvTable::typeInt},
        {MyCsv::timedOutHeader, CsvTable::typeBool},
        {MyCsv::winHeader, CsvTable::typeBool},
    };

    CsvTable csv(columns);
    SeriesTotal total = RunASeriesOfGames(args, csv);
    csv.PrintTable();

    OutputSeriesTotals(args.GetCount(), total);
}
