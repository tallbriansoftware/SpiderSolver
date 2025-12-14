#pragma once
#include "spidersolvercore/Model/MoveSingle.h"

#include <string>
#include <vector>

enum class ComboType {
    None, FlipRuns, Move2Runs, InsertRun,
    RemoveMiddleRun, TradeHolesA, TradeHolesB, SwapRuns,
    Deal
};


class MoveCombo
{
public:
    static MoveCombo Deal();
    static MoveCombo None();

public:
    MoveCombo();
    explicit MoveCombo(const MoveSingle& smove);
    MoveCombo(const std::vector<MoveSingle>& moveList);

public:
    bool IsValid() const;
    bool IsDeal() const;

    int Count() const;

    ComboType GetComboType() const;
    void SetComboType(ComboType type);

    MoveCombo& AddMove(const MoveSingle& move);
    MoveCombo& AddMove(const MoveCombo& move);

    const MoveSingle& GetMove(int index) const;

    const std::vector<MoveSingle>& GetMoves() const;

    bool operator==(const MoveCombo& rhs);
    bool operator!=(const MoveCombo& rhs);

private:
    std::vector<MoveSingle> m_moves;
    ComboType m_type;
};

