#pragma once

#include "spidersolvercore/Model/MoveCombo.h"

class Strategy;
class SpiderTableau;


class ScoredMove
{
public:
    ScoredMove(float score, const MoveCombo& move, const MoveCombo& pathDown);

    float GetScore() const;    
    const MoveCombo& GetMove() const;
    const MoveCombo& GetPath() const;

    float GetLocalScore() const;
    void SetLocalScore(const Strategy& strategy, const SpiderTableau& tableau);

private:
    float m_score;
    float m_localScore;
    MoveCombo m_move;
    MoveCombo m_pathDown;
};