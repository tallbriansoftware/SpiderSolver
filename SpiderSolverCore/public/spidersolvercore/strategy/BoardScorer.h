#pragma once

#include <string>
#include <vector>
#include <array>

class SpiderTableau;

class BoardScorer
{
public:
    BoardScorer();
    BoardScorer(const BoardScorer& bs) = default;
    float ComputeScore(const SpiderTableau& tableau) const;
    float MaxScore() const;

private:
    float m_suitedRunTerm;
    float m_holesTerm;
    float m_turnedCardsTerm;
    float m_fullPackTerm;
};