#include "spidersolvercore/strategy/SearchContext.h"

#include "spidersolvercore/model/Ancestry.h"
#include "spidersolvercore/model/MoveCombo.h"
#include "spidersolvercore/model/SpiderNode.h"

#include <string>
#include <assert.h>
#include <iostream>


SearchContext::SearchContext(int maxDepth, const Ancestry& ancestry, MoveFinderFunc func)
    : m_maxDepth(maxDepth)
    , m_parentList(std::make_unique<Ancestry>(ancestry))
    , m_moveFinderFunc(func)
{ }


SearchContext::~SearchContext()
{ }


int SearchContext::GetMaxDepth() const
{
    return m_maxDepth;
}


std::vector<MoveCombo> SearchContext::GetMoves(const SpiderTableau& tableau)
{
    return m_moveFinderFunc(tableau);
}


void SearchContext::AddParentPosition(const std::string& tabString)
{
    auto count = m_parentList->PushTableau(tabString);
 //   std::cout << "Saving Parent: " << count << " " << tabString << "\n";
}


void SearchContext::RemoveParentPosition(const std::string& tabString)
{
    auto listHeadString = m_parentList->PopTableau();
    assert(tabString == listHeadString);
//    std::cout << "Removing Parent: " << tabString << "\n";

}

namespace
{
    uint16_t Hash(const std::string& s)
    {
        uint16_t hash = 0;
        for (char c : s)
        {
            uint16_t highBits = (hash & 0xC000);
            hash += c;
            hash <<= 2;
            hash |= highBits >> 14;
        }
        return hash;
    }
}

bool SearchContext::IsAParentPosition(const std::string& tabString) const
{
//    std::cout << "Checking Parent: " << tabString << "\n";
    return (m_parentList->FindRepeatIndex(tabString) > 0);
}

bool SearchContext::TryFindSpiderNode(
    const std::string& tableauString,
    SpiderNode& node)
{
    auto itr = m_nodeMap.find(tableauString);
    if (itr != m_nodeMap.end())
    {
        int hash = Hash(tableauString);
 //       std::cout << " Hitting:  " << hash << " " << tableauString << "\n";

        node = itr->second;
        return true;
    }
    return false;
}

void SearchContext::AddSpiderNode(const SpiderNode& node)
{
    int hash = Hash(node.GetStringRep());
 //   std::cout << " Adding:  " << hash << " " << node.GetStringRep() << "\n";

#ifdef _DEBUG
    SpiderNode dummy;
    bool val = TryFindSpiderNode(node.GetStringRep(), dummy);
    assert(!val);
#endif
    m_nodeMap[node.GetStringRep()] = node;
}

bool SearchContext::RemoveSpiderNode(const std::string& tableauString)
{
    int hash = Hash(tableauString);
 //   std::cout << "Removing: " << hash << " " << tableauString << "\n";

    return 0 != m_nodeMap.erase(tableauString);
}

