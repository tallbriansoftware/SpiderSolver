#pragma once

#include "spidersolvercore/Model/SpiderTableau.h"

#include <vector>

class Ancestry
{
public:
    Ancestry();
    Ancestry(const SpiderTableau& tableau);
    Ancestry(const Ancestry& ancestry);
public:
    // Add a Spider Tableau to the ancestry
    int AddTableau(const SpiderTableau& tableau);

    int PushTableau(const std::string& tabString);
    std::string PopTableau();

    int GetLength() const;

    // Check the History for a matching Spider Tableau
    // Return the move number that is repeated.
    int FindRepeatIndex(const std::string& tableauString) const;

private:
    std::vector<std::string> GetHistory() const;

private:
    const Ancestry* m_parent;
    std::vector<std::string> m_history;
};