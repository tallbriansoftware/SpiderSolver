#pragma once

#include "spidersolvercore/Model/MoveCombo.h"

class MoveCombo;


class TreeMove
{
public:
    TreeMove();
    TreeMove(float score, const MoveCombo& move, const std::vector<MoveCombo>& path);
    TreeMove(float score, const MoveCombo& move, const MoveCombo& path);
    TreeMove(const TreeMove& treeMove) = default;

    float GetScore() const;
    MoveCombo GetMove() const;
    MoveCombo GetPathDown() const;

private:
    float m_score;
    MoveCombo m_move;
    MoveCombo m_pathDown;
};