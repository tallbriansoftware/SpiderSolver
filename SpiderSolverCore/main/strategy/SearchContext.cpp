#include "spidersolvercore/strategy/SearchContext.h"

#include "spidersolvercore/model/Ancestry.h"


SearchContext::SearchContext(int maxDepth, const Ancestry& ancestry)
    : m_maxDepth(maxDepth)
    , m_parentList(std::make_unique<Ancestry>(ancestry))
{ }


SearchContext::~SearchContext()
{ }


int SearchContext::GetMaxDepth() const
{
    return m_maxDepth;
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
