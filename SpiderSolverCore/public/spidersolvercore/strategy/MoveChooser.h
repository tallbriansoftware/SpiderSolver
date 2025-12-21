#pragma once

#include "spidersolvercore/model/Ancestry.h"
#include "spidersolvercore/strategy/Strategy.h"
#include "spidersolvercore/strategy/ScoredMove.h"

#include <vector>
#include <memory>

class ScoredMove;
class SpiderTableau;
class Strategy;


class MoveChooser
{
public:
    MoveChooser(
        std::shared_ptr<SpiderTableau> tableau,
        Strategy& strategy,
        int depth);

    MoveCombo ComputeBestMove();
    void CommitMove(const MoveCombo& move);
    int GetMoveNumber() const;

    const std::vector<ScoredMove>& GetAllChoices() const;
    const std::vector<MoveCombo>& GetDisregardedChoices() const;

private:
    std::shared_ptr<SpiderTableau> m_tableau;
    Strategy& m_strategy;
    Ancestry m_ancestry;
    int m_depth;

    // caching fields
    std::vector<ScoredMove> m_moveChoices;
    std::vector<MoveCombo> m_disregardedChoices;
};