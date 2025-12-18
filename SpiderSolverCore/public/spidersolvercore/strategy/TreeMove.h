#pragma once

#include "spidersolvercore/Model/MoveCombo.h"

class MoveCombo;


class TreeNode
{

public:
    TreeNode();
    TreeNode(const TreeNode& treeNode) = default;

    TreeNode(float score, const MoveCombo& move);

    float GetScore() const;
    MoveCombo GetMove() const;

private:
    float m_score;
    MoveCombo m_move;
};
