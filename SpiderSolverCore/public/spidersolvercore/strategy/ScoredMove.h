#pragma once

#include "spidersolvercore/Model/MoveCombo.h"

struct ScoredMove
{
    // Score result from making this move.
    float localScore;

    // One logical move, but it can be more than one "simple" move.
    // E.g.  a swap, or a move through a hole.
    MoveCombo move;

    // This is the score at the end of the path.
    float depthScore;

    // This is the path following this move that
    // ends in the node for the "depthScore"
    std::vector<MoveCombo> futurePath;

    ScoredMove(float localScore, const MoveCombo& move);
    ScoredMove(
        float localScore, const MoveCombo& move,
        float depthScore, const std::vector<MoveCombo>& futurePath);
};