#include "spidersolvercore/strategy/TreeMove.h"


TreeMove::TreeMove()
    : m_score(-1.0)
{ }


TreeMove::TreeMove(
    float score,
    const MoveCombo& move,
    const std::vector<MoveCombo>& path)
    : m_score(score)
    , m_move(move)
{
    for (MoveCombo cmove : path)
    {
        for (MoveSingle smove : cmove.GetMoves())
        {
            m_pathDown.AddMove(smove);
        }

    }
}

TreeMove::TreeMove(
    float score,
    const MoveCombo& move,
    const MoveCombo& path)
    : m_score(score)
    , m_move(move)
    , m_pathDown(path)
{
}


float TreeMove::GetScore() const
{
    return m_score;
}


MoveCombo TreeMove::GetMove() const
{
    return m_move;
}


MoveCombo TreeMove::GetPathDown() const
{
    return m_pathDown;
}
