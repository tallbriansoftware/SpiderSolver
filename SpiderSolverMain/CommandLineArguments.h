#pragma once

#include <string>
#include <vector>

class CommandLineArguments
{
public:
    CommandLineArguments(int argc, char** argv);
    bool Parse();

    void Usage();

    bool SetRandomSeeds(std::string seedRanges);
    std::vector<int> GetRandomSeeds() const;

    bool SetSuits(int suitNumber);
    int GetSuits() const;

    bool SetDisplay();
    bool GetDisplay() const;

    bool SetTreeDepths(std::string depthRanges);
    std::vector<int> GetTreeDepths() const;

    bool SetDealUp();
    bool GetDealUp() const;

    bool SetLimitSeconds(int seconds);
    int GetLimitSeconds() const;


private:
    const std::vector<std::string> m_argv;
    mutable std::vector<int> m_seeds;
    mutable std::vector<int> m_treeDepths;
    int m_suits;
    bool m_display;
    bool m_dealup;
    int m_limitSeconds;
};