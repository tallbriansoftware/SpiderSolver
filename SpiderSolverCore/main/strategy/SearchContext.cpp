#include "spidersolvercore/strategy/SearchContext.h"

#include "spidersolvercore/model/Ancestry.h"
#include "spidersolvercore/model/MoveCombo.h"


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
