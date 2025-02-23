#include "Helpers/DeckHelper.h"
#include "spidersolvercore/Model/Card.h"

namespace Cards
{
    Card DAS() { return Card(Rank::Ace, Suit::Spades); }
    Card D2S() { return Card(Rank::Two, Suit::Spades); }
    Card D3S() { return Card(Rank::Three, Suit::Spades); }
    Card D4S() { return Card(Rank::Four, Suit::Spades); }
    Card D5S() { return Card(Rank::Five, Suit::Spades); }
    Card D6S() { return Card(Rank::Six, Suit::Spades); }
    Card D7S() { return Card(Rank::Seven, Suit::Spades); }
    Card D8S() { return Card(Rank::Eight, Suit::Spades); }
    Card D9S() { return Card(Rank::Nine, Suit::Spades); }
    Card DTS() { return Card(Rank::Ten, Suit::Spades); }
    Card DJS() { return Card(Rank::Jack, Suit::Spades); }
    Card DQS() { return Card(Rank::Queen, Suit::Spades); }
    Card DKS() { return Card(Rank::King, Suit::Spades); }

    Card DAH() { return Card(Rank::Ace, Suit::Hearts); }
    Card D2H() { return Card(Rank::Two, Suit::Hearts); }
    Card D3H() { return Card(Rank::Three, Suit::Hearts); }
    Card D4H() { return Card(Rank::Four, Suit::Hearts); }
    Card D5H() { return Card(Rank::Five, Suit::Hearts); }
    Card D6H() { return Card(Rank::Six, Suit::Hearts); }
    Card D7H() { return Card(Rank::Seven, Suit::Hearts); }
    Card D8H() { return Card(Rank::Eight, Suit::Hearts); }
    Card D9H() { return Card(Rank::Nine, Suit::Hearts); }
    Card DTH() { return Card(Rank::Ten, Suit::Hearts); }
    Card DJH() { return Card(Rank::Jack, Suit::Hearts); }
    Card DQH() { return Card(Rank::Queen, Suit::Hearts); }
    Card DKH() { return Card(Rank::King, Suit::Hearts); }

    Card DAD() { return Card(Rank::Ace, Suit::Diamonds); }
    Card D2D() { return Card(Rank::Two, Suit::Diamonds); }
    Card D3D() { return Card(Rank::Three, Suit::Diamonds); }
    Card D4D() { return Card(Rank::Four, Suit::Diamonds); }
    Card D5D() { return Card(Rank::Five, Suit::Diamonds); }
    Card D6D() { return Card(Rank::Six, Suit::Diamonds); }
    Card D7D() { return Card(Rank::Seven, Suit::Diamonds); }
    Card D8D() { return Card(Rank::Eight, Suit::Diamonds); }
    Card D9D() { return Card(Rank::Nine, Suit::Diamonds); }
    Card DTD() { return Card(Rank::Ten, Suit::Diamonds); }
    Card DJD() { return Card(Rank::Jack, Suit::Diamonds); }
    Card DQD() { return Card(Rank::Queen, Suit::Diamonds); }
    Card DKD() { return Card(Rank::King, Suit::Diamonds); }

    Card DAC() { return Card(Rank::Ace, Suit::Clubs); }
    Card D2C() { return Card(Rank::Two, Suit::Clubs); }
    Card D3C() { return Card(Rank::Three, Suit::Clubs); }
    Card D4C() { return Card(Rank::Four, Suit::Clubs); }
    Card D5C() { return Card(Rank::Five, Suit::Clubs); }
    Card D6C() { return Card(Rank::Six, Suit::Clubs); }
    Card D7C() { return Card(Rank::Seven, Suit::Clubs); }
    Card D8C() { return Card(Rank::Eight, Suit::Clubs); }
    Card D9C() { return Card(Rank::Nine, Suit::Clubs); }
    Card DTC() { return Card(Rank::Ten, Suit::Clubs); }
    Card DJC() { return Card(Rank::Jack, Suit::Clubs); }
    Card DQC() { return Card(Rank::Queen, Suit::Clubs); }
    Card DKC() { return Card(Rank::King, Suit::Clubs); }


    Card UAS() { return Card(Rank::Ace, Suit::Spades).TurnFaceUp(); }
    Card U2S() { return Card(Rank::Two, Suit::Spades).TurnFaceUp(); }
    Card U3S() { return Card(Rank::Three, Suit::Spades).TurnFaceUp(); }
    Card U4S() { return Card(Rank::Four, Suit::Spades).TurnFaceUp(); }
    Card U5S() { return Card(Rank::Five, Suit::Spades).TurnFaceUp(); }
    Card U6S() { return Card(Rank::Six, Suit::Spades).TurnFaceUp(); }
    Card U7S() { return Card(Rank::Seven, Suit::Spades).TurnFaceUp(); }
    Card U8S() { return Card(Rank::Eight, Suit::Spades).TurnFaceUp(); }
    Card U9S() { return Card(Rank::Nine, Suit::Spades).TurnFaceUp(); }
    Card UTS() { return Card(Rank::Ten, Suit::Spades).TurnFaceUp(); }
    Card UJS() { return Card(Rank::Jack, Suit::Spades).TurnFaceUp(); }
    Card UQS() { return Card(Rank::Queen, Suit::Spades).TurnFaceUp(); }
    Card UKS() { return Card(Rank::King, Suit::Spades).TurnFaceUp(); }

    Card UAH() { return Card(Rank::Ace, Suit::Hearts).TurnFaceUp(); }
    Card U2H() { return Card(Rank::Two, Suit::Hearts).TurnFaceUp(); }
    Card U3H() { return Card(Rank::Three, Suit::Hearts).TurnFaceUp(); }
    Card U4H() { return Card(Rank::Four, Suit::Hearts).TurnFaceUp(); }
    Card U5H() { return Card(Rank::Five, Suit::Hearts).TurnFaceUp(); }
    Card U6H() { return Card(Rank::Six, Suit::Hearts).TurnFaceUp(); }
    Card U7H() { return Card(Rank::Seven, Suit::Hearts).TurnFaceUp(); }
    Card U8H() { return Card(Rank::Eight, Suit::Hearts).TurnFaceUp(); }
    Card U9H() { return Card(Rank::Nine, Suit::Hearts).TurnFaceUp(); }
    Card UTH() { return Card(Rank::Ten, Suit::Hearts).TurnFaceUp(); }
    Card UJH() { return Card(Rank::Jack, Suit::Hearts).TurnFaceUp(); }
    Card UQH() { return Card(Rank::Queen, Suit::Hearts).TurnFaceUp(); }
    Card UKH() { return Card(Rank::King, Suit::Hearts).TurnFaceUp(); }

    Card UAD() { return Card(Rank::Ace, Suit::Diamonds).TurnFaceUp(); }
    Card U2D() { return Card(Rank::Two, Suit::Diamonds).TurnFaceUp(); }
    Card U3D() { return Card(Rank::Three, Suit::Diamonds).TurnFaceUp(); }
    Card U4D() { return Card(Rank::Four, Suit::Diamonds).TurnFaceUp(); }
    Card U5D() { return Card(Rank::Five, Suit::Diamonds).TurnFaceUp(); }
    Card U6D() { return Card(Rank::Six, Suit::Diamonds).TurnFaceUp(); }
    Card U7D() { return Card(Rank::Seven, Suit::Diamonds).TurnFaceUp(); }
    Card U8D() { return Card(Rank::Eight, Suit::Diamonds).TurnFaceUp(); }
    Card U9D() { return Card(Rank::Nine, Suit::Diamonds).TurnFaceUp(); }
    Card UTD() { return Card(Rank::Ten, Suit::Diamonds).TurnFaceUp(); }
    Card UJD() { return Card(Rank::Jack, Suit::Diamonds).TurnFaceUp(); }
    Card UQD() { return Card(Rank::Queen, Suit::Diamonds).TurnFaceUp(); }
    Card UKD() { return Card(Rank::King, Suit::Diamonds).TurnFaceUp(); }

    Card UAC() { return Card(Rank::Ace, Suit::Clubs).TurnFaceUp(); }
    Card U2C() { return Card(Rank::Two, Suit::Clubs).TurnFaceUp(); }
    Card U3C() { return Card(Rank::Three, Suit::Clubs).TurnFaceUp(); }
    Card U4C() { return Card(Rank::Four, Suit::Clubs).TurnFaceUp(); }
    Card U5C() { return Card(Rank::Five, Suit::Clubs).TurnFaceUp(); }
    Card U6C() { return Card(Rank::Six, Suit::Clubs).TurnFaceUp(); }
    Card U7C() { return Card(Rank::Seven, Suit::Clubs).TurnFaceUp(); }
    Card U8C() { return Card(Rank::Eight, Suit::Clubs).TurnFaceUp(); }
    Card U9C() { return Card(Rank::Nine, Suit::Clubs).TurnFaceUp(); }
    Card UTC() { return Card(Rank::Ten, Suit::Clubs).TurnFaceUp(); }
    Card UJC() { return Card(Rank::Jack, Suit::Clubs).TurnFaceUp(); }
    Card UQC() { return Card(Rank::Queen, Suit::Clubs).TurnFaceUp(); }
    Card UKC() { return Card(Rank::King, Suit::Clubs).TurnFaceUp(); }
}
