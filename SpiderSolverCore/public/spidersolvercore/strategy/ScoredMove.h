#pragma once

#include "spidersolvercore/Model/MoveCombo.h"

class Strategy;
class SpiderTableau;


class ScoredMove
{
public:
    ScoredMove(float score, const MoveCombo& move, std::vector<MoveCombo> pathDown);

    float GetScore() const;    
    const MoveCombo& GetMove() const;
    const std::vector<MoveCombo> GetPath() const;

    float GetLocalScore() const;
    void SetLocalScore(const Strategy& strategy, const SpiderTableau& tableau);

private:
    float m_score;
    float m_localScore;
    MoveCombo m_move;
    std::vector<MoveCombo> m_pathDown;
};