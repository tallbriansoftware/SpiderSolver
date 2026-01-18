#pragma once


#include <vector>

class MoveSingle;
class MoveCombo;
class SpiderTableau;
class SpiderStack;

namespace MoveFindingFunc
{
    std::vector<MoveCombo> Normal(const SpiderTableau& tableau);
    std::vector<MoveCombo> Any(const SpiderTableau& tableau);
    std::vector<MoveCombo> JustHoleFilling(const SpiderTableau& tableau);
};

class MoveFinder
{
public:
    std::vector<MoveCombo> Normal(const SpiderTableau& tableau);
    std::vector<MoveCombo> FillHoleThenNorma(const SpiderTableau& tableau);
};
