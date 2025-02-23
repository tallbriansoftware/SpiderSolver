#include "spidersolvercore/strategy/ScoredMove.h"

ScoredMove::ScoredMove(float localScore, const MoveCombo& move)
    : localScore(localScore)
    , move(move)
    , depthScore(0.0)
    , futurePath()
{ }

ScoredMove::ScoredMove(
    float localScore, const MoveCombo& move,
    float depthScore, const std::vector<MoveCombo>& futurePath)
    : localScore(localScore)
    , move(move)
    , depthScore(depthScore)
    , futurePath(futurePath)
{ }