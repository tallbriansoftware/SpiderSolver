#include "spidersolvercore/logic/MoveFinder.h"

#include "MoveFinderSimple.h"
#include "MoveFinderWithHole.h"

#include "spidersolvercore/model/MoveCombo.h"
#include "spidersolvercore/model/MoveSingle.h"

#include <vector>


std::vector<MoveCombo> MoveFinder::Normal(const SpiderTableau& tableau)
{
    std::vector<MoveSingle> moves;

    MoveFinderSimple::AddSimpleMoves(moves, tableau);
    MoveFinderSimple::AddColorUpMoves(moves, tableau);

    auto comboMoves = ConvertSingleMovesToComboMoves(moves);

    MoveFinderWithHole::AddAllOneHoleMoves(comboMoves, tableau);

    return comboMoves;
}


std::vector<MoveCombo> MoveFinder::NormalAndHoleFilling(const SpiderTableau& tableau)
{
    std::vector<MoveSingle> moves;

    MoveFinderSimple::AddSimpleMoves(moves, tableau);
    MoveFinderSimple::AddColorUpMoves(moves, tableau);
    MoveFinderSimple::AddHoleFillingMoves(moves, tableau);

    auto comboMoves = ConvertSingleMovesToComboMoves(moves);

    MoveFinderWithHole::AddAllOneHoleMoves(comboMoves, tableau);

    return comboMoves;
}


std::vector<MoveCombo> MoveFinder::ConvertSingleMovesToComboMoves(
    const std::vector<MoveSingle>& smoves)
{
    std::vector<MoveCombo> cmoves;
    for (auto& move : smoves)
        cmoves.push_back(MoveCombo(move));
    return cmoves;
}


//std::vector<MoveCombo> MoveFinder::AppendMoveVecs(
//    const std::vector<MoveCombo>& a,
//    const std::vector<MoveCombo>& b)
//{
//    std::vector<MoveCombo> moves;
//    moves = a;
//    moves.insert(a.end(), b.begin(), b.end());
//    return moves;
//}

