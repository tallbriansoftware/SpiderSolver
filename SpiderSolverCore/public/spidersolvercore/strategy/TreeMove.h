#pragma once

#include "spidersolvercore/Model/MoveCombo.h"

class MoveCombo;


class TreeMove
{
public:
    TreeMove();
    TreeMove(float score, const MoveCombo& move);
    TreeMove(const MoveCombo& move, const TreeMove& bestChild);
    float GetScore() const;
    MoveCombo GetMove() const;
    std::vector<MoveCombo> GetPathUp() const;

private:
    float m_score;
    MoveCombo m_move;
    std::vector<MoveCombo> m_pathUp;
};