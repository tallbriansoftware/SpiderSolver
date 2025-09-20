#pragma once


#include <vector>

class MoveSingle;
class MoveCombo;
class SpiderTableau;
class SpiderStack;

namespace MoveFinder
{
    std::vector<MoveCombo> Normal(const SpiderTableau& tableau);
    std::vector<MoveCombo> NormalAndHoleFilling(const SpiderTableau& tableau);


    // Some Helpers that might be of public usefullness
    //
    std::vector<MoveCombo> ConvertSingleMovesToComboMoves(
        const std::vector<MoveSingle>& smoves);

    //std::vector<MoveCombo> AppendMoveVecs(
    //    const std::vector<MoveCombo>& a,
    //    const std::vector<MoveCombo>& b);
};