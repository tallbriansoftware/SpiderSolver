#include "spidersolvercore/strategy/SearchContext.h"

#include "spidersolvercore/model/Ancestry.h"
#include "spidersolvercore/model/MoveCombo.h"
#include "spidersolvercore/model/SpiderNode.h"

#include <string>
#include <assert.h>


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
    m_parentList->PushTableau(tabString);
}


void SearchContext::RemoveParentPosition(const std::string& tabString)
{
    m_parentList->PopTableau();
}


bool SearchContext::IsAParentPosition(const std::string& tabString) const
{
    return m_parentList->IsRepeat(tabString);
}

bool SearchContext::TryFindSpiderNode(
    const std::string& tableauString,
    SpiderNode& node)
{
    auto itr = m_nodeMap.find(tableauString);
    if (itr != m_nodeMap.end())
    {
        node = itr->second;
        return true;
    }
    return false;
}

void SearchContext::AddSpiderNode(const SpiderNode& node)
{
#ifdef _DEBUG
    SpiderNode dummy;
    assert(!TryFindSpiderNode(node.GetStringRep(), dummy));
#endif
    m_nodeMap[node.GetStringRep()] = node;
}

bool SearchContext::RemoveSpiderNode(const std::string& tableauString)
{
    return 0 != m_nodeMap.erase(tableauString);
}

