#include "spidersolvercore/Model/Ancestry.h"


Ancestry::Ancestry()
    : m_parent(nullptr)
{}


Ancestry::Ancestry(const SpiderTableau& tableau)
    : m_parent(nullptr)
{
    AddTableau(tableau);
}


Ancestry::Ancestry(const Ancestry& parent)
    : m_parent(&parent)
{
}


std::vector<std::string> Ancestry::GetHistory() const
{
    if (m_parent == nullptr)
        return m_history;

    auto parenthistory = m_parent->GetHistory();
    std::vector<std::string> history;
    history.insert(history.end(), parenthistory.begin(), parenthistory.end());
    history.insert(history.end(), m_history.begin(), m_history.end());
    return history;
}


int Ancestry::AddTableau(const SpiderTableau& tableau)
{
    return PushTableau(tableau.GetTableauString());
}


int Ancestry::PushTableau(const std::string& tabString)
{
    m_history.push_back(tabString);
    return (int)m_history.size();
}

std::string Ancestry::PopTableau()
{
    auto top = *(m_history.end() - 1);
    m_history.pop_back();
    return top;
}

int Ancestry::GetLength() const
{
    int parentLength = (m_parent == nullptr) ? 0 : m_parent->GetLength();
    return (int)m_history.size() + parentLength;
}


int Ancestry::FindRepeatIndex(const std::string& tableauString) const
{
    int parentLength = (m_parent == nullptr) ? 0 : m_parent->GetLength();

    for (int i = static_cast<int>(m_history.size() - 1); i >= 0; i--)
    {
        if (tableauString == m_history[i])
            return i + parentLength;
    }
    if (m_parent != nullptr)
        return m_parent->FindRepeatIndex(tableauString);

    return -1;
}
