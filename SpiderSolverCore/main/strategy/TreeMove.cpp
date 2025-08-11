#include "spidersolvercore/strategy/TreeMove.h"


TreeMove::TreeMove()
{ }


TreeMove::TreeMove(float score, const MoveCombo& move)
    : m_score(score)
    , m_move(move)
    , m_pathUp()
{
    m_pathUp.push_back(move);
}


TreeMove::TreeMove(const MoveCombo& move, const TreeMove& bestChild)
    : m_score(bestChild.GetScore())
    , m_move(move)
    , m_pathUp(bestChild.GetPathUp())
{
    m_pathUp.push_back(move);
}


float TreeMove::GetScore() const
{
    return m_score;
}


MoveCombo TreeMove::GetMove() const
{
    return m_move;
}


std::vector<MoveCombo> TreeMove::GetPathUp() const
{
    return m_pathUp;
}
