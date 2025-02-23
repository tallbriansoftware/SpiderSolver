#include "spidersolvercore/Model/SpiderString.h"

#include "spidersolvercore/Model/Card.h"
#include "spidersolvercore/Core/SpiderException.h"
#include "spidersolvercore/Model/SpiderStack.h"
#include "spidersolvercore/Model/SpiderTableau.h"

#include <algorithm>

std::string SpiderString::Create(const SpiderStack& stack)
{
    constexpr char DownCardCode[] = "x";
    constexpr char ColumnCode[] = "/";
    constexpr char Suits[] = "SHDC";
    constexpr char Ranks[] = "A23456789TJQK";
    const std::string alphNum("0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ");

    std::string result(ColumnCode);
    int downCount = 0;
    for (auto& card : stack.GetCards())
    {
        if (!card.IsFaceUp())
            downCount += 1;
    }
    if (downCount > 0)
    {
        result += alphNum[downCount];
        result += DownCardCode;
        downCount = 0;
    }
    for (auto& card : stack.GetCards())
    {
        if (!card.IsFaceUp())
            continue;
        result += Ranks[static_cast<int>(card.getRank())];
        result += Suits[static_cast<int>(card.getSuit())];
    }
    return result;
}

namespace
{
    void SortSpiderStrings(std::vector<const std::string*>& strings)
    {
        std::sort(begin(strings), end(strings), [](const std::string* lhs, const std::string* rhs)
            {
                return *lhs < *rhs;
            });
    }
}

std::string SpiderString::Create(const SpiderTableau& tableau)
{
    std::vector<const std::string*> spiderStrings;
    for (auto& stack : tableau.GetStacks())
    {
        const std::string& stackString = stack.GetStackString();
        spiderStrings.push_back(&stackString);
    }

    SortSpiderStrings(spiderStrings);

    int size = 0;
    for (auto stackString : spiderStrings)
        size += (int)stackString->size();

    std::string result;
    result.reserve(size);
    for (auto stackString : spiderStrings)
        result += *stackString;

    return result;
}
