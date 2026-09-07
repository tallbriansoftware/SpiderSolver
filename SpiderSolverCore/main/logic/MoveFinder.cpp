#include "spidersolvercore/logic/MoveFinder.h"

#include "MoveFinderSimple.h"
#include "MoveFinderWithHole.h"

#include "spidersolvercore/model/MoveCombo.h"
#include "spidersolvercore/model/MoveSingle.h"

#include <vector>


namespace
{
    std::vector<MoveCombo> ConvertSingleMovesToComboMoves(
        const std::vector<MoveSingle>& smoves)
    {
        std::vector<MoveCombo> cmoves;
        for (auto& move : smoves)
            cmoves.push_back(MoveCombo(move));
        return cmoves;
    }
}

std::vector<MoveCombo> MoveFinder::Normal(const SpiderTableau& tableau)
{
    std::vector<MoveSingle> moves;

    MoveFinderSimple::AddSimpleMoves(moves, tableau);
    MoveFinderSimple::AddColorUpMoves(moves, tableau);

    auto comboMoves = ConvertSingleMovesToComboMoves(moves);

    MoveFinderWithHole::AddOneHoleUsingMoves(comboMoves, tableau);

    return comboMoves;
}


std::vector<MoveCombo> MoveFinder::Any(const SpiderTableau& tableau)
{
    std::vector<MoveSingle> moves;

    MoveFinderSimple::AddSimpleMoves(moves, tableau);
    MoveFinderSimple::AddColorUpMoves(moves, tableau);
    MoveFinderSimple::AddHoleFillingMoves(moves, tableau);

    // Simple run splitting moves are far too common and not often usefull.
    //MoveFinderSimple::AddRunSplittingMoves(moves, tableau);

    auto comboMoves = ConvertSingleMovesToComboMoves(moves);

    MoveFinderWithHole::AddOneHoleUsingMoves(comboMoves, tableau);

    return comboMoves;
}
std::vector<MoveCombo> MoveFinder::JustHoleFilling(const SpiderTableau& tableau)
{
    std::vector<MoveSingle> moves;

    MoveFinderSimple::AddHoleFillingMoves(moves, tableau);
    auto comboMoves = ConvertSingleMovesToComboMoves(moves);
    return comboMoves;
}