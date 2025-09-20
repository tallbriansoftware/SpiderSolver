#pragma once

#include "spidersolvercore/logic/MoveFinderFunc.h"

#include <functional>
#include <memory>
#include <string>
#include <vector>


class Ancestry;
class MoveCombo;
class SpiderTableau;


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

private:
    int m_maxDepth;
    std::unique_ptr<Ancestry> m_parentList;
    MoveFinderFunc m_moveFinderFunc;
};
