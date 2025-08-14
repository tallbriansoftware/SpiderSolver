#include "spidersolvercore/Model/MoveCombo.h"

#include "spidersolvercore/Model/MoveSingle.h"

#include <assert.h>
#include <string>
#include <vector>

MoveCombo::MoveCombo() = default;

MoveCombo::MoveCombo(const MoveSingle& smove)
{
    m_moves.push_back(smove);
    m_type = ComboType::None;
}

MoveCombo::MoveCombo(const std::vector<MoveSingle>& moves)
    : m_moves(moves)
    , m_type(ComboType::None)
{
}

int MoveCombo::Count() const
{
    return (int)m_moves.size();
}

ComboType MoveCombo::GetComboType() const
{
    return m_type;
}

void MoveCombo::SetComboType(ComboType type)
{
    m_type = type;
}

MoveCombo& MoveCombo::AddMove(const MoveSingle& smove)
{
    m_moves.push_back(smove);
    return *this;
}

MoveCombo& MoveCombo::AddMove(const MoveCombo& move)
{
    auto& innerMoves = move.GetMoves();
    m_moves.insert(m_moves.end(), innerMoves.begin(), innerMoves.end());
    return *this;
}

const MoveSingle& MoveCombo::GetMove(int index) const
{
    if (index < 0)
        index = (int)m_moves.size() + index;
    return m_moves[index];
}

const std::vector<MoveSingle>& MoveCombo::GetMoves() const
{
    return m_moves;
}

bool MoveCombo::operator==(const MoveCombo& rhs)
{
    if (Count() != rhs.Count())
        return false;

    for (int i = 0; i < Count(); ++i)
    {
        if (m_moves[i] != rhs.m_moves[i])
            return false;
    }
    return true;
}

bool MoveCombo::operator!=(const MoveCombo& rhs)
{
    return !(*this == rhs);
}

