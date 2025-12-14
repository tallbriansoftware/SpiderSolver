#include "spidersolvercore/Model/MoveCombo.h"

#include "spidersolvercore/Model/MoveSingle.h"

#include <assert.h>
#include <string>
#include <vector>


MoveCombo::MoveCombo()
    : m_type(ComboType::None)
{
}

MoveCombo::MoveCombo(const MoveSingle& smove)
{
    m_moves.push_back(smove);
    m_type = ComboType::None;
}

MoveCombo MoveCombo::Deal()
{
    MoveCombo deal;
    deal.m_type = ComboType::Deal;
    return deal;
}

MoveCombo MoveCombo::None()
{
    MoveCombo deal;
    deal.m_type = ComboType::None;
    return deal;
}

MoveCombo::MoveCombo(const std::vector<MoveSingle>& moves)
    : m_moves(moves)
    , m_type(ComboType::None)
{
}

bool MoveCombo::IsValid() const
{
    // type == None is not valid.
    return (m_moves.size() != 0 || m_type == ComboType::Deal);
}

bool MoveCombo::IsDeal() const
{
    return (m_type == ComboType::Deal);
}

int MoveCombo::Count() const
{
    if (m_type == ComboType::Deal)
        return 1;
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

