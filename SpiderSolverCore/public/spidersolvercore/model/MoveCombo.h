#pragma once
#include "spidersolvercore/Model/MoveSingle.h"

#include <string>
#include <vector>

enum class ComboType {
    None, Single, Multi, FlipRuns, Move2Runs, InsertRun,
    RemoveMiddleRun, TradeHolesA, TradeHolesB, SwapRuns,
    SplitRun, 
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
    MoveCombo(const std::vector<MoveSingle>& moveList, ComboType type);

public:
    bool IsValid() const;
    bool IsDeal() const;

    int Count() const;

    ComboType GetComboType() const;

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

