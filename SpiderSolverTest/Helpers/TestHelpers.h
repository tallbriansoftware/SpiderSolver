#pragma once

#include <string>
#include <vector>

class SpiderTableau;
class SpiderStack;
class Card;
class MoveCombo;
class MoveSingle;
class ScoredMove;

void PrintTableau(const SpiderTableau& tableau);

void PrintMoves(const SpiderTableau& tableau, const std::vector<MoveCombo>& moves);

void PrintMoves(const SpiderTableau& tableau, const std::vector<MoveSingle>& moves);

void PrintMoves(const SpiderTableau& tableau, const std::vector<ScoredMove>& scoredMoves);

void SetStack(SpiderStack& stack, std::vector<Card> cards);

bool IsSameMove(const MoveSingle& actual, const MoveSingle& expected);

bool IsSameMoves(const std::vector<MoveSingle>& actual, const std::vector<MoveSingle>& expected);
