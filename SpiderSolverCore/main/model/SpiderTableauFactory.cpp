#include "spidersolvercore/Model/SpiderTableauFactory.h"

#include "spidersolvercore/Model/Card.h"
#include "spidersolvercore/Model/SpiderDeckFactory.h"
#include "spidersolvercore/model/SpiderTableau.h"


std::shared_ptr<SpiderTableau> SpiderTableauFactory::Create(int numSuits, int seed)
{
    auto deck = SpiderDeckFactory::Create(numSuits, seed);
    return Create(*deck);
}


std::shared_ptr<SpiderTableau> SpiderTableauFactory::Create(SpiderStack& deck)
{
    auto tableau = std::make_shared<SpiderTableau>(deck);
    return tableau;
}

namespace
{
    void TurnAllCardsFaceUp(SpiderTableau& tableau)
    {
        for (auto& stack : tableau.GetMutableStacks())
        {
            for (auto& card : stack.GetMutableCards())
                card.TurnFaceUp();
        }
    }
}


std::shared_ptr<SpiderTableau> SpiderTableauFactory::CreateAllUp(int numSuits, int seed)
{
    auto deck = SpiderDeckFactory::Create(numSuits, seed);
    auto tableau = Create(*deck);
    TurnAllCardsFaceUp(*tableau);
    return tableau;
}
