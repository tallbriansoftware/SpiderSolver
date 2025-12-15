#pragma once

#include "spidersolvercore/Model/MoveCombo.h"

class MoveCombo;


class TreeMove
{
public:
    TreeMove();
    TreeMove(float score, const MoveCombo& move, const std::vector<MoveCombo> path);

    float GetScore() const;
    MoveCombo GetMove() const;
    std::vector<MoveCombo> GetPathDown() const;

private:
    float m_score;
    MoveCombo m_move;
    std::vector<MoveCombo> m_pathDown;
};