#pragma once

#include "spidersolvercore/Model/DoTurnCard.h"
#include "spidersolvercore/Model/CardEnums.h"

class Card;
class SpiderTableau;
class MoveSingle;
class MoveCombo;
class SpiderStack;
class TableauStats;

#include <vector>
#include <string>

namespace SpiderPrint
{
    char PrintSuit(Suit suit);
    char PrintRank(Rank rank);

    std::string PrintCard(const Card& card);
    std::string PrintCardWithFacing(const Card& card);
    std::string PrintStackForInit(const SpiderStack& deck);
    std::vector<std::string> PrintTableau(const SpiderTableau& tableau);

    std::string PrintPlainMove(const MoveSingle& move);
    std::string PrintPlainMoves(const std::vector<MoveSingle>& moves);

    std::string PrintBookMove(const SpiderTableau& tableau, const MoveSingle& move, DoTurnCard turn);
    std::string PrintBookMove(const SpiderTableau& tableau, const MoveCombo& move, DoTurnCard turn);

    std::string PrintBookMoves(const SpiderTableau& tableau, const std::vector<MoveSingle>& move, DoTurnCard turn);
    std::string PrintBookMoves(const SpiderTableau& tableau, const std::vector<MoveCombo>& move, DoTurnCard turn);

    std::string PrintTableauStatsHeaders();
    std::string PrintTableauStats(const TableauStats& score);
}