#pragma once

#include <memory>
#include <string>


class Ancestry;


class SearchContext
{
public:

public:
    SearchContext(int maxDepth, const Ancestry& ancestry);
    ~SearchContext();

    int GetMaxDepth() const;

    bool IsAParentPosition(const std::string& tabString) const;
    void AddParentPosition(const std::string& tabString);
    void RemoveParentPosition(const std::string& tableau);

private:
    int m_maxDepth;
    std::unique_ptr<Ancestry> m_parentList;
};
