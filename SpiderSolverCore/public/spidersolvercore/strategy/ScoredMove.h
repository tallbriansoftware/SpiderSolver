#pragma once

#include "spidersolvercore/Model/MoveCombo.h"

class Strategy;
class SpiderTableau;


class ScoredMove
{
public:
    ScoredMove(
        float score,
        const MoveCombo& move);

    float GetScore() const;
    const MoveCombo& GetMove() const;

    float GetLocalScore() const;
    void SetLocalScore(const Strategy& strategy, const SpiderTableau& tableau);

private:
    float m_score;
    float m_localScore;
    MoveCombo m_move;
};