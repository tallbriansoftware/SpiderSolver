#pragma once

#include <string>
#include <vector>

class SpiderTableau;


class BoardScorer
{
public:
    BoardScorer();
    BoardScorer(const BoardScorer& bs) = default;
    float ComputeScore(const SpiderTableau& tableau) const;
    float MaxScore() const;

    void SetHolesTerm(float term);
    void SetTurnedCardsTerm(float term);

    std::vector<float> GetModifiedTerms() const;
    std::vector<std::string> GetModifiedTermNames() const;

private:
    unsigned int m_setTerms;

    float m_suitedRunTerm;
    float m_holesTerm;
    float m_turnedCardsTerm;
    float m_fullPackTerm;
};