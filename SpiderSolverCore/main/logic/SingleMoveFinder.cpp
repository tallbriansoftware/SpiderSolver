#include "spidersolvercore/Logic/SingleMoveFinder.h"

#include "spidersolvercore/Logic/SingleMoveFindingFuncs.h"
#include "spidersolvercore/Model/SpiderStack.h"
#include "spidersolvercore/Model/SpiderTableau.h"
#include "spidersolvercore/model/MoveSingle.h"
#include "spidersolvercore/model/MoveCombo.h"


// SimpleMoves:  Find normal simple moves.
// -- Excludes breaking suited run
//
std::vector<MoveSingle> SingleMoveFinder::AllSingleMoves(const SpiderTableau& tableau)
{
    std::vector<MoveSingle> moves;
    AddSimpleMoves(moves, tableau);
    AddColorUpMoves(moves, tableau);
    // Exclude rank-splitting moves.
    // AddSuitedRankSplittingMoves(moves, tableau);
    AddHoleFillingMoves(moves, tableau);

    return moves;
}

std::vector<MoveCombo> SingleMoveFinder::AllSingleMovesAsCombos(const SpiderTableau& tableau)
{
    std::vector<MoveSingle> smoves = SingleMoveFinder::AllSingleMoves(tableau);
    std::vector<MoveCombo> moves;
    for (auto& move : smoves)
        moves.push_back(MoveCombo(move));
    return moves;
}


int SingleMoveFinder::AddSimpleMoves(std::vector<MoveSingle>& moves, const SpiderTableau& tableau)
{
    return SingleMoveFindingFuncs::FindMoves(moves, tableau, SingleMoveFindingFuncs::SimpleMove);
}


int SingleMoveFinder::AddColorUpMoves(std::vector<MoveSingle>& moves, const SpiderTableau& tableau)
{
    return SingleMoveFindingFuncs::FindMoves(moves, tableau, SingleMoveFindingFuncs::ColorUpMove);
}


int SingleMoveFinder::AddSuitedRankSplittingMoves(std::vector<MoveSingle>& moves, const SpiderTableau& tableau)
{
    return SingleMoveFindingFuncs::FindMoves(moves, tableau, SingleMoveFindingFuncs::SuitedRunSplittingMove);
}


int SingleMoveFinder::AddHoleFillingMoves(std::vector<MoveSingle>& moves, const SpiderTableau& tableau)
{
    int holeIndex = tableau.FindFirstHoleIndex();
    if (holeIndex < 0)
        return 0;

    const auto& destStack = tableau.GetStack(holeIndex);
    return SingleMoveFindingFuncs::FindMovesForDestinationStack(moves, destStack, tableau, SingleMoveFindingFuncs::HoleFillingMove);
}
