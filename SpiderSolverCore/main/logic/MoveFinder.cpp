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

std::vector<MoveCombo> MoveFindingFunc::Normal(const SpiderTableau& tableau)
{
    std::vector<MoveSingle> moves;

    MoveFinderSimple::AddSimpleMoves(moves, tableau);
    MoveFinderSimple::AddColorUpMoves(moves, tableau);

    auto comboMoves = ConvertSingleMovesToComboMoves(moves);

    MoveFinderWithHole::AddAllOneHoleMoves(comboMoves, tableau);

    return comboMoves;
}


std::vector<MoveCombo> MoveFindingFunc::Any(const SpiderTableau& tableau)
{
    std::vector<MoveSingle> moves;

    MoveFinderSimple::AddSimpleMoves(moves, tableau);
    MoveFinderSimple::AddColorUpMoves(moves, tableau);
    MoveFinderSimple::AddHoleFillingMoves(moves, tableau);
    //MoveFinderSimple::AddRunSplittingMoves(moves, tableau);

    auto comboMoves = ConvertSingleMovesToComboMoves(moves);

    MoveFinderWithHole::AddAllOneHoleMoves(comboMoves, tableau);

    return comboMoves;
}
std::vector<MoveCombo> MoveFindingFunc::JustHoleFilling(const SpiderTableau& tableau)
{
    std::vector<MoveSingle> moves;

    MoveFinderSimple::AddHoleFillingMoves(moves, tableau);

    auto comboMoves = ConvertSingleMovesToComboMoves(moves);

    return comboMoves;
}