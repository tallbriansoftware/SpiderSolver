#include "spidersolvercore/strategy/TreeMove.h"

std::vector<MoveSingle> SinglesVecFromCombosVec(const std::vector<MoveCombo>& moves)
{
    std::vector<MoveSingle> result;

    for (MoveCombo cmove : moves)
    {
        for (MoveSingle smove : cmove.GetMoves())
        {
            result.push_back(smove);
        }
    }
    return result;
}


TreeNode::TreeNode()
    : m_score(-1.0)
{ }


TreeNode::TreeNode(
    float score,
    const MoveCombo& move)
    : m_score(score)
    , m_move(move)
{
}

float TreeNode::GetScore() const
{
    return m_score;
}

MoveCombo TreeNode::GetMove() const
{
    return m_move;
}
