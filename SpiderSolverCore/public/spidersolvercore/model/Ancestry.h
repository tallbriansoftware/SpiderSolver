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
    void AddTableau(const SpiderTableau& tableau);

    void PushTableau(const std::string& tabString);
    void PopTableau();

    int GetLength() const;

    // Check the History for a matching Spider Tableau
    bool IsRepeat(const std::string& tableauString) const;

    std::vector<std::string> GetStringHistory() const;

private:
    std::vector<std::string> m_stringHistory;
    const Ancestry* m_parent;
};