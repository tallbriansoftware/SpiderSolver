#include "spidersolvercore/Utils/SpiderPrint.h"

#include "spidersolvercore/Model/Card.h"
#include "spidersolvercore/Model/CardEnums.h"
#include "spidersolvercore/Model/SpiderStack.h"
#include "spidersolvercore/Model/SpiderTableau.h"
#include "spidersolvercore/Model/MoveSingle.h"
#include "spidersolvercore/Model/MoveCombo.h"
#include "spidersolvercore//Model/TableauStats.h"

#include <string>
#include <iostream>
#include <iomanip>
#include <sstream>


char SpiderPrint::PrintSuit(Suit suit)
{
    static const char suitChars[] = "SHDC";
    return suitChars[(int)suit];
}


char SpiderPrint::PrintRank(Rank rank)
{
    static const char rankChars[] = "A23456789TJQK";
    return rankChars[(int)rank];
}


std::string SpiderPrint::PrintCard(const Card& card)
{
    std::string cardString;
    cardString += PrintRank(card.getRank());
    cardString += PrintSuit(card.getSuit());

    return cardString;
}


std::string SpiderPrint::PrintCardWithFacing(const Card& card)
{
    if (!card.IsFaceUp())
        return "::";
    else
        return PrintCard(card);
}


// Used for generating test code.
std::string SpiderPrint::PrintStackForInit(const SpiderStack& stack)
{
    std::stringstream buffer;
    std::string seperator = "";
    for (auto& card : stack.GetCards())
    {
        std::string side = "U";
        std::string value;
        if (card.IsFaceUp())
            value = PrintCard(card);
        else
        {
            side = "D";
            Card flipped = card;
            flipped.TurnFaceUp();
            value = PrintCard(flipped);
        }
        buffer << seperator << side << value << "()";
        seperator = ", ";
    }
    return buffer.str();
}


std::vector<std::string> SpiderPrint::PrintTableau(const SpiderTableau& tableau)
{
    std::array<int, SpiderTableau::NUM_STACKS> lengths;
    int maxLength = 0;
    for (int stackIndex = 0; stackIndex < SpiderTableau::NUM_STACKS; ++stackIndex)
    {
        lengths[stackIndex] = tableau.GetStack(stackIndex).Count();
        if (lengths[stackIndex] > maxLength)
            maxLength = lengths[stackIndex];
    }

    std::vector<std::string> lines;

    std::stringstream buffer;
    buffer << "  ";
    for (int i = 0; i < SpiderTableau::NUM_STACKS; ++i)
        buffer << " " << i << " ";
    lines.push_back(buffer.str());

    for (int i = 0; i < maxLength; ++i)
    {
        buffer.str("");
        buffer << i;
        if (i < 10)
            buffer << " ";
        for (int stackIndex = 0; stackIndex < SpiderTableau::NUM_STACKS; ++stackIndex)
        {
            auto stack = tableau.GetStack(stackIndex);
            if (i < stack.Count())
            {
                buffer << PrintCardWithFacing(stack.GetCard(i)) << " ";
            }
            else
            {
                buffer << " - ";
            }
        }
        lines.push_back(buffer.str());
    }
    buffer.str("");
    buffer << "Deals-Left=" << tableau.GetDealsRemaining()
        << ", Packs-Off=" << tableau.GetPacksRemoved();
    lines.push_back(buffer.str());
    return lines;
}


namespace
{
    std::string PrintSuitedRun(const SpiderStack& stack, int head, int tail)
    {
        std::stringstream buffer;

        Card headCard = stack.GetCard(head);
        Card tailCard = stack.GetCard(tail);

        buffer << SpiderPrint::PrintCard(headCard);
        if (head != tail)
            buffer << SpiderPrint::PrintRank(tailCard.getRank());

        return buffer.str();
    }
}


std::string SpiderPrint::PrintPlainMove(const MoveSingle& move)
{
    std::stringstream buffer;

    int srcNo = move.FromStack();
    int srcIndex = move.FromIndex();
    int destNo = move.DestStack();
    int targetIndex = move.DestIndex() - 1;

    buffer << destNo << "<-" << srcNo << "(" << srcIndex << ")";
    return buffer.str();
}


std::string SpiderPrint::PrintPlainMoves(const std::vector<MoveSingle>& moves)
{
    std::string all;
    for (auto& move : moves)
    {
        all += PrintPlainMove(move);
        all += "  ";
    }
    return all;
}


std::string SpiderPrint::PrintBookMove(const SpiderTableau& tableau, const MoveSingle& smove, DoTurnCard turn)
{
    std::stringstream buffer;

    int srcNo = smove.FromStack();
    int srcIndex = smove.FromIndex();
    int destNo = smove.DestStack();
    int targetIndex = smove.DestIndex() - 1;

    auto& src = tableau.GetStack(srcNo);
    auto& dest = tableau.GetStack(destNo);

    auto holeAdjustment = 0;
    buffer << destNo << "{";
    if (dest.IsEmpty())
    {
        holeAdjustment = -1;
        buffer << "E" << tableau.GetHoleCount() + holeAdjustment << "}";
    }
    else
    {
        buffer << PrintSuitedRun(dest, dest.GetRunHead(0), targetIndex) << "}";
    }

    buffer << "<-";

    int srcTailIndex = src.Count() - 1;
    Card srcHeadCard = tableau.GetStack(srcNo).GetCard(srcIndex);
    Card srcTailCard = tableau.GetStack(srcNo).GetCard(-1);

    buffer << srcNo << "{" << PrintSuitedRun(src, srcIndex, srcTailIndex);
    if (srcIndex == 0)
    {
        holeAdjustment += 1;
        buffer << "=E" << tableau.GetHoleCount() + holeAdjustment;
    }
    else
    {
        auto& prevCard = src.GetCard(srcIndex - 1);
        if (!prevCard.IsFaceUp())
        {
            if (turn == DoTurnCard::No)
                buffer << "=?";
            else
            {
                Card tmp = prevCard;
                tmp.TurnFaceUp();
                buffer << "=" << PrintCard(tmp);
            }
        }
    }
    buffer << "}";
    if (!dest.IsEmpty())
    {
        // If the move was suited
        Card targetCard = dest.GetCard(targetIndex);
        if (srcHeadCard.getSuit() == targetCard.getSuit())
        {
            // look for a K->A pack
            int targetHeadIndex = dest.GetRunHead(0);
            if (dest.GetCard(targetHeadIndex).getRank() == Rank::King
                && srcTailCard.getRank() == Rank::Ace)
            {
                buffer << "=X" << tableau.GetPacksRemoved() + 1;
                if (targetHeadIndex == 0)
                {
                    holeAdjustment += 1;
                    buffer << ",E" << tableau.GetHoleCount() + holeAdjustment;
                }
            }

        }
    }
    return buffer.str();
}


namespace
{
    std::string ComboTypeString(ComboType type)
    {
        switch (type)
        {
        case ComboType::None: return "none"; break;
        case ComboType::FlipRuns: return "flip-runs"; break;
        case ComboType::TradeHolesA: return "trade-holes-A"; break;
        case ComboType::TradeHolesB: return "trade-holes-B"; break;
        case ComboType::Move2Runs: return "move-two-runs"; break;
        case ComboType::SwapRuns: return "swap-runs"; break;
        case ComboType::RemoveMiddleRun: return "remove-middle-run"; break;
        case ComboType::InsertRun: return "insert-run"; break;

        default: return "missing"; break;
        }
    }

    std::string TryPrintExchange_throughHole(
        const SpiderTableau& tableau,
        const MoveCombo& move,
        DoTurnCard turn)
    {
        if (move.Count() == 3)
        {
            const MoveSingle& move0 = move.GetMove(0);
            const MoveSingle& move1 = move.GetMove(1);
            const MoveSingle& move2 = move.GetMove(2);

            // Exchanges look like  A<-B, B<-C, C<-A
            // B and C swap through A.  Where A is a hole
            if (move0.FromStack() != move1.DestStack())
                return {};
            if (move1.FromStack() != move2.DestStack())
                return {};
            if (move2.FromStack() != move0.DestStack())
                return {};

            if (!tableau.GetStack(move0.DestStack()).IsEmpty())
                return {};

            auto& src0 = tableau.GetStack(move0.FromStack());
            auto& src1 = tableau.GetStack(move1.FromStack());

            std::string first = PrintSuitedRun(src0, move0.FromIndex(), src0.Count() - 1);
            std::string second = PrintSuitedRun(src1, move1.FromIndex(), src1.Count() - 1);

            std::stringstream buffer;
            buffer << move0.FromStack() << "{" + first + "} <=> "
                << move1.FromStack() << "{" + second + "}";
            buffer << " " << ComboTypeString(move.GetComboType());
            return buffer.str();
        }
        return {};
    }

    std::string TryPrintMoveTwoRuns_throughHole(
        const SpiderTableau& tableau,
        const MoveCombo& move,
        DoTurnCard turn)
    {
        if (move.Count() == 3)
        {
            const MoveSingle& move0 = move.GetMove(0);
            const MoveSingle& move1 = move.GetMove(1);
            const MoveSingle& move2 = move.GetMove(2);

            // looks like  A-->B A-->C, B-->C
            // A moves two runs onto C, where B is a hole.
            // or at least A is a place where A can move.
            if (move0.FromStack() != move1.FromStack())
                return {};
            if (move1.DestStack() != move2.DestStack())
                return {};
            if (move0.DestStack() != move2.FromStack())
                return {};

            if (!tableau.GetStack(move0.DestStack()).IsEmpty())
                return {};

            auto& src0 = tableau.GetStack(move0.FromStack());
            auto& dest1 = tableau.GetStack(move1.DestStack());
            int destHead = dest1.GetRunHead(0);

            std::string destString = PrintSuitedRun(dest1, destHead, move1.DestIndex() - 1);
            std::string srcString1 = PrintSuitedRun(src0, move1.FromIndex(), move0.FromIndex() - 1);
            std::string srcString2 = PrintSuitedRun(src0, move0.FromIndex(), src0.Count() - 1);

            std::stringstream buffer;
            buffer << move1.DestStack() << "{" << destString << "}<-"
                << move0.FromStack() << "{" << srcString1 << "-" << srcString2 << "}";
            buffer << " " << ComboTypeString(move.GetComboType());
            return buffer.str();;
        }
        return {};
    }
}

std::string SpiderPrint::PrintBookMove(
    const SpiderTableau& originalTableau,
    const MoveCombo& move,
    DoTurnCard turn)
{
    std::string comboString;
    std::string seperator = "";
    if (move.Count() == 1)
        comboString = PrintBookMove(originalTableau, move.GetMove(0), turn);
    else
    {
        SpiderTableau tableau(originalTableau);

        std::string special;

        special = TryPrintExchange_throughHole(tableau, move, turn);
        if (special.size() > 0)
            return special;

        special = TryPrintMoveTwoRuns_throughHole(tableau, move, turn);
        if (special.size() > 0)
            return special;

        for (const auto& smove : move.GetMoves())
        {
            comboString += seperator + PrintBookMove(tableau, smove, turn);
            seperator = ", ";
            tableau.DoMove(smove, DoTurnCard::No);
        }
        comboString += " " + ComboTypeString(move.GetComboType());
    }
    return comboString;
}

std::string SpiderPrint::PrintBookMoves(const SpiderTableau& tableau, const std::vector<MoveSingle>& moves, DoTurnCard turn)
{
    MoveCombo cmove(moves);
    return PrintBookMove(tableau, cmove, DoTurnCard::No);
}

std::string SpiderPrint::PrintBookMoves(const SpiderTableau& tableau, const std::vector<MoveCombo>& moves, DoTurnCard turn)
{
    MoveCombo megaCombo;
    for (auto cmove : moves)
    {
        megaCombo.AddMove(cmove);
    }
    return PrintBookMove(tableau, megaCombo, DoTurnCard::No);
}


std::string SpiderPrint::PrintTableauStatsHeaders()
{
    return " Tur Hol Pck Del";
}


std::string SpiderPrint::PrintTableauStats(const TableauStats& score)
{
    std::stringstream buffer;
    buffer.width(4);
    buffer
        << score.GetNumberOfTurnedCards() << std::setw(4)
        << score.GetNumberOfHoles() << std::setw(4)
        << score.GetNumberOfCompletedPacks() << std::setw(4)
        << score.GetNumberOfDealsRemaining() << std::setw(4);

    return buffer.str();
}
