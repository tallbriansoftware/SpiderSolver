#include "CommandLineArguments.h"

#include <functional>
#include <iostream>
#include <sstream>
#include <string>

namespace
{
    std::vector<std::string> ConvertToStrings(int argc, char** argv)
    {
        std::vector<std::string> strings;
        for (int i = 0; i < argc; ++i)
            strings.push_back({ argv[i] });
        return strings;
    }

    bool StartsWith(std::string prefix, std::string testString)
    {
        if (testString.length() < prefix.length())
            return false;
        for (int i = 0; i < (int)prefix.length(); ++i)
            if (prefix[i] != testString[i])
                return false;
        return true;
    }

    enum class OptionType { Flag, Int, String };
    struct Option
    {
        const char* shortName;
        const char* longName;
        OptionType optType;

        std::function<bool(CommandLineArguments& args,
            int value, const std::string strArg)> process;
    };


    bool SetRandomSeedThunk(CommandLineArguments& that, int seed, std::string seedRanges)
    {
        return that.SetRandomSeeds(seedRanges);
    }

    bool SetSuitsThunk(CommandLineArguments& that, int suits, std::string dummy)
    {
        return that.SetSuits(suits);
    }

    bool SetDisplayThunk(CommandLineArguments& that, int dummy1, std::string dummy2)
    {
        return that.SetDisplay();
    }

    bool SetDealUpThunk(CommandLineArguments& that, int dummy1, std::string dummy2)
    {
        return that.SetDealUp();
    }

    bool SetTreeDepthThunk(CommandLineArguments& that, int depth, std::string dummy)
    {
        return that.SetTreeDepth(depth);
    }

    bool SetLimitSecondsThunk(CommandLineArguments& that, int seconds, std::string dummy)
    {
        return that.SetLimitSeconds(seconds);
    }

    const std::vector<Option> OptionsTable = {
        { "-r", "--randomSeeds", OptionType::String, SetRandomSeedThunk },
        { "-s", "--suits", OptionType::Int, SetSuitsThunk },
        { "-d", "--display", OptionType::Flag, SetDisplayThunk },
        { "-u", "--dealup", OptionType::Flag, SetDealUpThunk },
        { "-t", "--treeDepth", OptionType::Int, SetTreeDepthThunk },
        { "-l", "--limitSeconds", OptionType::Int, SetLimitSecondsThunk },

    };

    int FindLongNamedOption(std::string arg)
    {
        for (int i = 0; i < (int)OptionsTable.size(); ++i)
        {
            if (OptionsTable[i].longName == arg)
                return i;
        }
        return -1;
    }

    int FindShortNamedOption(std::string arg)
    {
        for (int i = 0; i < (int)OptionsTable.size(); ++i)
        {
            if (OptionsTable[i].shortName == arg)
                return i;
        }
        return -1;
    }

    std::vector<int> ParseRanges(std::string stringRanges)
    {
        std::stringstream ss(stringRanges);
        std::string subString;
        std::vector<std::string> majorSections;

        while (std::getline(ss, subString, ','))
            majorSections.push_back(subString);

        std::vector<int> values;
        for (auto section : majorSections)
        {
            size_t idx = 0;
            int val0 = std::stoi(section, &idx);
            if (idx == section.size())
            {
                values.push_back(val0);
                continue;
            }
            if(section[idx] = '-')
            {
                std::string backHalf = section.substr(idx + 1);
                int val1 = std::stoi(backHalf, &idx);
                if (idx == backHalf.size())
                {
                    for (int v = val0; v <= val1; v++)
                        values.push_back(v);
                    continue;
                }
                values.clear();
                return values;  // clear the list and return on error
            }
            values.clear();
            return values;  // clear the list and return on error
        }
        return values;  // clear the list and return on error
    }
}

CommandLineArguments::CommandLineArguments(int argc, char** argv)
    : m_argv(ConvertToStrings(argc, argv))
    , m_seeds()
    , m_suits(4)
    , m_count(1)
    , m_treeDepth(0)
    , m_display(false)
    , m_dealup(false)
    , m_limitSeconds(0)
{
}

void CommandLineArguments::Usage()
{
    std::cerr << "Usage: " << m_argv[0] << std::endl;
    for (auto& opt : OptionsTable)
    {
        std::cerr << "\t" << opt.shortName << " or " << opt.longName;
        if (opt.optType == OptionType::Int)
            std::cerr << " <int-arg>";
        else if (opt.optType == OptionType::String)
            std::cerr << " <arg>";
        std::cerr << std::endl;
    }
}

bool CommandLineArguments::Parse()
{
    int argc = (int)m_argv.size();
    for (int argIndex = 1; argIndex < argc; ++argIndex)
    {
        int index = -1;
        std::string flag = m_argv[argIndex];
        if (StartsWith("--", flag))
            index = FindLongNamedOption(flag);
        else if (StartsWith("-", flag))
            index = FindShortNamedOption(flag);

        if (index < 0)
        {
            std::cerr << "Unknown Option: '" << flag << "'" << std::endl;
            return false;;
        }
        const Option& option = OptionsTable[index];
        auto arg_proc = option.process;
        if (option.optType == OptionType::Flag)
        {
            arg_proc(*this, 1, "");
        }
        else
        {
            if (argIndex + 1 >= argc)
            {
                std::cerr << "Missing Argument to Option: '" << flag << "'" << std::endl;
                return false;
            }
            argIndex += 1;  // Modifing the look varaible;
            const std::string& stringArg = m_argv[argIndex];

            bool isValid = false;
            if (option.optType == OptionType::Int)
            {
                size_t parseIndex = 0;
                int intArg = std::stoi(stringArg, &parseIndex);
                if (parseIndex != stringArg.size())
                    std::cerr << "Argument not an integer.  ";
                else
                    isValid = arg_proc(*this, intArg, stringArg);
            }
            else if (option.optType == OptionType::String)
            {
                isValid = arg_proc(*this, 0, stringArg);
            }

            if (!isValid)
            {
                std::cerr << "Bad Argument '" << stringArg << "' to Option: '"
                          << flag << "'" << std::endl;
                return false;
            }
        }
    }
    return true;
}

int CommandLineArguments::GetCount() const
{
    return (int)m_seeds.size();
    //return m_count;
}

bool CommandLineArguments::SetRandomSeeds(std::string seedRanges)
{
    m_seeds = ParseRanges(seedRanges);
    return (m_seeds.size() != 0);
}

std::vector<int> CommandLineArguments::GetRandomSeeds() const
{
    if (m_seeds.size() == 0)
    {
        time_t t = time(nullptr);
        srand((int)t);
        int dummy = rand();
        m_seeds.push_back(rand());
    }
    return m_seeds;
}

bool CommandLineArguments::SetSuits(int suits)
{
    if (suits == 1 || suits == 2 || suits == 4)
    {
        m_suits = suits;
        return true;
    }
    return false;
}

int CommandLineArguments::GetSuits() const
{
    return m_suits;
}

bool CommandLineArguments::SetDisplay()
{
    m_display = true;
    return true;
}

bool CommandLineArguments::GetDisplay() const
{
    return m_display;
}


bool CommandLineArguments::SetDealUp()
{
    m_dealup = true;
    return true;
}

bool CommandLineArguments::GetDealUp() const
{
    return m_dealup;
}

bool CommandLineArguments::SetTreeDepth(int depth)
{
    m_treeDepth = depth;
    return true;
}

int CommandLineArguments::GetTreeDepth() const
{
    return m_treeDepth;
}

bool CommandLineArguments::SetLimitSeconds(int seconds)
{
    m_limitSeconds = seconds;
    return true;
}

int CommandLineArguments::GetLimitSeconds() const
{
    return m_limitSeconds;
}
