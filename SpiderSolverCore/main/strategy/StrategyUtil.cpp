#include "spidersolvercore/Strategy/StrategyUtil.h"

#include "spidersolvercore/Model/Ancestry.h"
#include "spidersolvercore/Model/MoveCombo.h"
#include "spidersolvercore/Model/SpiderTableau.h"
#include "spidersolvercore/strategy/ScoredMove.h"

#include <algorithm>

namespace
{

    std::vector<std::string> GetTableauStringsFromListOfMoves(
        const std::vector<MoveCombo>& moves,
        const SpiderTableau& parentTableau)
    {
        SpiderTableau tableau(parentTableau);
        std::vector<std::string> tableauStrings;

        for (auto& move : moves)
        {
            SpiderTableau::SavePoint save(tableau);
            tableau.DoMove(move, DoTurnCard::No);
            tableauStrings.push_back(tableau.GetTableauString());
        }
        return tableauStrings;
    }


    std::vector<std::string> GetTableauStringsFromListOfMoves(
        const std::vector<MoveSingle>& moves,
        const SpiderTableau& parentTableau)
    {
        SpiderTableau tableau(parentTableau);
        std::vector<std::string> tableauStrings;

        for (auto& move : moves)
        {
            SpiderTableau::SavePoint save(tableau);
            tableau.DoMove(move, DoTurnCard::No);
            tableauStrings.push_back(tableau.GetTableauString());
        }
        return tableauStrings;
    }


    std::vector<int> GetIndicesOfMovesThatAreAncestors(
        const std::vector<std::string>& tableauStrings,
        const Ancestry& ancestry)
    {
        std::vector<int> dups;
        for (int i = 0; i < (int)tableauStrings.size(); ++i)
        {
            auto& tableauString = tableauStrings[i];
            if (ancestry.IsRepeat(tableauString))
                dups.push_back(i);
        }
        return dups;
    }

    // Find groups of combo moves that have the same result.
    // Ie.  combo1: a to b and c to d SAME AS combo2: c to d and a to b
    std::vector<std::vector<int>> FindGroupsOfDuplicates(
        const std::vector<std::string>& tableauStrings)
    {
        using StringEntry = std::pair<int, const std::string*>;
        std::vector<StringEntry> indexedStrings;
        for (int i = 0; i < (int)tableauStrings.size(); ++i)
            indexedStrings.emplace_back(i, &tableauStrings[i]);

        std::sort(indexedStrings.begin(), indexedStrings.end(), [](StringEntry& a, StringEntry& b)
            {
                return *a.second < *b.second;
            });

        std::vector<std::vector<int>> dups;
        std::vector<int> group;

        for (int i = 1; i < (int)indexedStrings.size(); ++i)
        {
            int iBefore = i - 1;
            auto& elem0 = indexedStrings[iBefore];
            auto& elem1 = indexedStrings[i];
            if (*elem0.second == *elem1.second)
            {
                if (group.size() == 0)
                    group.push_back(elem0.first);
                group.push_back(elem1.first);
            }
            else
            {
                if (group.size() != 0)
                {
                    dups.push_back(group);
                    group.clear();
                }
            }
        }
        if (group.size() != 0)
            dups.push_back(group);

        return dups;
    }
}


void StrategyUtil::SortLocalMoves(std::vector<ScoredMove>& moves)
{
    std::sort(begin(moves), end(moves),
        [](const ScoredMove& lhs, const ScoredMove& rhs)
        {
            // return the greator score.
            // unless the scores are tied then return the shorter move.
            if (lhs.localScore == rhs.localScore)
                return lhs.move.Count() < rhs.move.Count();
            return lhs.localScore > rhs.localScore;
        });
}


std::vector<MoveSingle> StrategyUtil::RemoveRepeats(
    const std::vector<MoveSingle>& moves,
    const SpiderTableau& tableau,
    const Ancestry& ancestry)
{
    // 
    auto moveTableauStrings = GetTableauStringsFromListOfMoves(moves, tableau);
    std::vector<int> ancestorDups = GetIndicesOfMovesThatAreAncestors(moveTableauStrings, ancestry);

    // Build a list of flags that mark a move as duplicate, and should be removed.
    std::vector<bool> dupFlags(moves.size(), false);

    // Build a bitmap marking the duplicates.
    // Start with the moves that are duplicates of ancestor positions.
    for (int dupIndex : ancestorDups)
        dupFlags[dupIndex] = true;

    std::vector<MoveSingle> retMoves;
    for (int i = 0; i < (int)moves.size(); ++i)
    {
        if (dupFlags[i] == false)
        {
            retMoves.push_back(moves[i]);
        }
    }
    return retMoves;
}


std::vector<MoveCombo> StrategyUtil::RemoveRepeats(
    const std::vector<MoveCombo>& moves,
    const SpiderTableau& parentTableau,
    const Ancestry& ancestry)
{
    // 
    auto moveTableauStrings = GetTableauStringsFromListOfMoves(moves, parentTableau);
    std::vector<std::vector<int>> moveDupGroups = FindGroupsOfDuplicates(moveTableauStrings);
    std::vector<int> ancestorDups = GetIndicesOfMovesThatAreAncestors(moveTableauStrings, ancestry);

    // Build a list of flags that mark a move as duplicate, and should be removed.
    std::vector<bool> dupFlags(moves.size(), false);

    // Build a bitmap marking the duplicates.
    // Start with the moves that are duplicates of ancestor positions.
    for (int dupIndex : ancestorDups)
        dupFlags[dupIndex] = true;

    for (auto& dupGroup : moveDupGroups)
    {
        // If the first member of a duplicate move group is already flagged 
        //   then the entire dup group matches an ancestor and skip the entire group.
        // If the first member of a duplicate move group is NOT a ancestor duplicate
        //   then find the shortest of move of the group, keep that one, and strike all the others.
        if (dupFlags[dupGroup[0]] == false)
        {
            // Find the best version of the move in the group.
            int shortest = 99;
            int indexOfShortest = -1;
            for (auto dupElement : dupGroup)
            {
                auto& move = moves[dupElement];
                if (move.Count() < shortest)
                {
                    shortest = move.Count();
                    indexOfShortest = dupElement;
                }
            }

            // Mark all but the shortest one as dups.
            for (int k : dupGroup)
            {
                if (k != indexOfShortest)
                    dupFlags[k] = true;
            }
        }
    }

    std::vector<MoveCombo> retMoves;
    for (int i = 0; i < (int)moves.size(); ++i)
    {
        if (dupFlags[i] == false)
        {
            retMoves.push_back(moves[i]);
        }
    }
    return retMoves;
}

