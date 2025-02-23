#include "spidersolvercore/Model/SpiderDeckFactory.h"

#include "spidersolvercore/Core/SpiderException.h"
#include "spidersolvercore/Model/SpiderConstants.h"
#include "spidersolvercore/Model/SpiderStack.h"

#include "spidersolvercore/Model/Card.h"

#include <memory>
#include <vector>


std::shared_ptr<SpiderStack> SpiderDeckFactory::Create(int numSuits)
{
    return Create(numSuits, 42);
}


std::shared_ptr<SpiderStack> SpiderDeckFactory::Create(int numSuits, int seed)
{
    srand(seed);

    if (numSuits != 1 && numSuits != 2 && numSuits != 4)
        throw SpiderException("Number of Suits must be 1, 2, or 4");

    std::vector<Card> sortedCards;

    // Generate a vector of all the right cards (sorted)
    for (int i = 0; i < NUM_CARDS; ++i)
    {
        Rank rank(static_cast<Rank>(i % NUM_RANKS));
        Suit suit(static_cast<Suit>((i / NUM_RANKS) % numSuits));
        Card card(rank, suit);
        sortedCards.push_back(card);
    }

    auto deck = std::make_shared<SpiderStack>();
    // randomly place the sorted cards into the Shuffled Deck.
    for (int i = 0; i < NUM_CARDS; i++)
    {
        int r = rand();     // returns numbers < 2^15
        r = (((r >> 8) & 0xFF) | ((r << 8) & 0xFF00));  // FUD re: swap low order bits.
        r %= sortedCards.size();
        Card card = sortedCards[r];
        sortedCards.erase(sortedCards.begin() + r);
        deck->AddNewCard(card);
    }
    return deck;
}

