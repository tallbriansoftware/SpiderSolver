#pragma once

#include "spidersolvercore/strategy/TreeMove.h"

#include <string>

class BoardScorer;
class SpiderTableau;


class SpiderNode
{
public:
    SpiderNode();
    SpiderNode(int depth, const std::string& tableauString, float score);

    int GetDepth() const;
    const std::string& GetStringRep() const;
    float GetScore() const;

private:
    int m_depth;
    std::string m_stringRep;
    float m_score;
};