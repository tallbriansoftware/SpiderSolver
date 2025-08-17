#pragma once

#include <tuple>
#include <string>
#include <vector>

class SpiderTableau;
class TableauStats;
class CommandLineArguments;
struct ScoredMove;
class Strategy;

struct BoardResult
{
    int moveCount;
    float score;
    int searchDepth;
    int evals;
    int64_t usecs;
    bool won;

    BoardResult()
        : moveCount(0)
        , score(0.0)
        , searchDepth(0)
        , evals(0)
        , usecs(0)
        , won(false)
    { }
};

void OutputBoard(
    const std::string& leadPadding,
    const SpiderTableau& tableau,
    const TableauStats& tStats);

void OutputScoredMoves(
    const SpiderTableau& tableau,
    const std::vector<ScoredMove>& scoredMoves);

BoardResult RunOneBoard(
    const CommandLineArguments& args,
    int seed,
    Strategy& strategy,
    int depth);
