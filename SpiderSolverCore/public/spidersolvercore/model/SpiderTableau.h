#pragma once

#include "spidersolvercore/Model/SpiderStack.h"
#include "spidersolvercore/Model/DoTurnCard.h"
#include "spidersolvercore/Model/SpiderString.h"
#include "spidersolvercore/Model/MoveReversible.h"

#include <array>

class MoveSingle;
class MoveCombo;

class SpiderTableau
{
public:
    static constexpr int NUM_STACKS = 10;

public:
    SpiderTableau();
    explicit SpiderTableau(const SpiderStack& deck);
    SpiderTableau(const SpiderTableau& tableau);
    SpiderTableau& operator=(const SpiderTableau& tableau);

    ~SpiderTableau();

    void InitWithDeck(SpiderStack& stack);

    void EnsureTopTurnedUp();
    bool DealTurn();
    void DoMove(const MoveSingle& move, DoTurnCard turnCard);
    void DoMove(const MoveCombo& move, DoTurnCard turnCard);

    const Card& PeekTopCardAt(int stackIndex) const;
    const Card& PeekCardAt(int stackIndex, int cardIndex) const;

    const std::array<SpiderStack, NUM_STACKS>& GetStacks() const;

    const SpiderStack& GetStack(int index) const;
    int IndexOf(const SpiderStack& stack) const;
    int GetDealsRemaining() const;
    bool CanDeal() const;
    int GetPacksRemoved() const;
    int GetHoleCount() const;
    int FindFirstHoleIndex() const;
    bool IsWon() const;

    // for debugging.
    void MakeStackStrings();

    bool operator==(const SpiderTableau& rhs) const;
    bool operator!=(const SpiderTableau& rhs) const;

    const std::string& GetTableauString() const;

    // for testing
    std::array<SpiderStack, NUM_STACKS>& GetMutableStacks();

    class SavePoint
    {
    public:
        SavePoint(SpiderTableau& tableau);
        ~SavePoint();
    private:
        SpiderTableau* m_tableau;
        int m_index;
    };


private:
    int GetSavePoint();
    bool RestoreToSavePoint(int savePoint);

    int MoveStackSize() const;

    void RemoveFullPack(SpiderStack& stack);
    void ReturnFullPack(SpiderStack& stack);
    void DealStartPosition();
    void UndoMove(const MoveReversible& move);

private:
    SpiderStack m_stock;
    SpiderStack m_completedPacks;
    std::array<SpiderStack, NUM_STACKS> m_stacks;
    std::vector<MoveReversible> m_moveStack;
    mutable std::string m_string;
};
