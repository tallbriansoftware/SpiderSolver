#pragma once

#include "spidersolvercore/strategy/TreeMove.h"

#include <string>

class BoardScorer;
class SpiderTableau;


class SpiderNode
{
public:
    SpiderNode();
    SpiderNode(int depth, const std::string& tableauString, const TreeMove& treeMove);

    int GetDepth() const;
    const std::string& GetStringRep() const;
    float GetScore() const;
    MoveCombo GetPathDown() const;

private:
    int m_depth;
    std::string m_stringRep;
    TreeMove m_treeMove;
};