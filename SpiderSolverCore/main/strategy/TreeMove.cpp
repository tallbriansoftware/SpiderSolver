#include "spidersolvercore/strategy/TreeMove.h"


TreeMove::TreeMove()
    : m_score(-1.0)
{ }


TreeMove::TreeMove(float score, const MoveCombo& move, const std::vector<MoveCombo> path)
    : m_score(score)
    , m_move(move)
    , m_pathDown(path)
{
    m_pathDown.push_back(move);
}


float TreeMove::GetScore() const
{
    return m_score;
}


MoveCombo TreeMove::GetMove() const
{
    return m_move;
}


std::vector<MoveCombo> TreeMove::GetPathDown() const
{
    return m_pathDown;
}
