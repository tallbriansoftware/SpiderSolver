#pragma once

#include "spidersolvercore/logic/MoveFinderFunc.h"

#include <functional>
#include <memory>
#include <string>
#include <vector>
#include <unordered_map>


class Ancestry;
class MoveCombo;
class SpiderTableau;
class SpiderNode;


class SearchContext
{
public:

public:
    SearchContext(int maxDepth, const Ancestry& ancestry, MoveFinderFunc func);
    ~SearchContext();

    int GetMaxDepth() const;
    std::vector<MoveCombo> GetMoves(const SpiderTableau& tableau);

    bool IsAParentPosition(const std::string& tabString) const;
    void AddParentPosition(const std::string& tabString);
    void RemoveParentPosition(const std::string& tableau);

    bool TryFindSpiderNode(const std::string& tableauString, SpiderNode& node);
    void AddSpiderNode(const SpiderNode& node);
    bool RemoveSpiderNode(const std::string& tableauString);

private:
    int m_maxDepth;
    std::unique_ptr<Ancestry> m_parentList;
    MoveFinderFunc m_moveFinderFunc;
    std::unordered_map<std::string, SpiderNode> m_nodeMap;
};
