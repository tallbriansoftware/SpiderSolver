#include "spidersolvercore/Strategy/StrategyUtil.h"

#include "spidersolvercore/Model/Ancestry.h"
#include "spidersolvercore/Model/MoveCombo.h"
#include "spidersolvercore/Model/SpiderTableau.h"
#include "spidersolvercore/strategy/ScoredMove.h"
#include "spidersolvercore/strategy/Strategy.h"

#include <algorithm>
#include <assert.h>


void StrategyUtil::SortTiedBestMoves(
    std::vector<ScoredMove>& moves,
    const Strategy& strategy,
    const SpiderTableau& tableau)
{
    if (moves.size() < 2)
        return;

#ifdef _DEBUG
    // We assume the list is already sorted.
    auto first = moves.begin();
    for (auto second = first+1; second != moves.end(); first++, second++)
    {
        assert(first->GetScore() >= second->GetScore());
    }
#endif
    // Collect the first group of equal valued moves.
    // and sort them based on locaScore (first move only).

    // Scan for the first run of equal scored moves.
    auto one = moves.begin();
    auto next = one + 1;
    for(; next != moves.end(); next++)
    {
        if (one->GetScore() != next->GetScore())
            break;
    }

    // Resort the first run of equal deep-scored moves
    // based on their local-score.
    std::sort(begin(moves), next,
        [&strategy, &tableau](ScoredMove& lhs, ScoredMove& rhs)
        {
            if(lhs.GetLocalScore() == -1.0)
                lhs.SetLocalScore(strategy, tableau);
            if (rhs.GetLocalScore() == -1.0)
                rhs.SetLocalScore(strategy, tableau);

            return lhs.GetLocalScore() > rhs.GetLocalScore();
        });
}

