#include "spidersolvercore/Model/Ancestry.h"


Ancestry::Ancestry()
    : m_parent(nullptr)
{}


Ancestry::Ancestry(const SpiderTableau& tableau)
    : m_parent(nullptr)
{
    AddTableau(tableau);
}


Ancestry::Ancestry(const Ancestry& ancestry)
{
    m_parent = &ancestry;
}


std::vector<std::string> Ancestry::GetStringHistory() const
{
    if (m_parent == nullptr)
        return m_stringHistory;

    auto parentStringHistory = m_parent->GetStringHistory();
    std::vector<std::string> stringHistory;
    stringHistory.insert(stringHistory.end(), parentStringHistory.begin(), parentStringHistory.end());
    stringHistory.insert(stringHistory.end(), m_stringHistory.begin(), m_stringHistory.end());
    return stringHistory;
}


void Ancestry::AddTableau(const SpiderTableau& tableau)
{
    PushTableau(tableau.GetTableauString());
}


void Ancestry::PushTableau(const std::string& tabString)
{
    m_stringHistory.push_back(tabString);
}


void Ancestry::PopTableau()
{
    auto size = m_stringHistory.size();
    m_stringHistory.resize(size - 1);
}

int Ancestry::GetLength() const
{
    int parentLength = (m_parent == nullptr) ? 0 : m_parent->GetLength();
    return (int)m_stringHistory.size() + parentLength;
}

bool Ancestry::IsRepeat(const std::string& tableauString) const
{
    for (int i = static_cast<int>(m_stringHistory.size() - 1); i >= 0; i--)
    {
        if (tableauString == m_stringHistory[i])
            return true;
    }

    if (m_parent != nullptr && m_parent->IsRepeat(tableauString))
        return true;

    return false;
}
