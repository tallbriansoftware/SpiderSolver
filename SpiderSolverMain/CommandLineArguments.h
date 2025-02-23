#pragma once

#include <string>
#include <vector>

class CommandLineArguments
{
public:
    CommandLineArguments(int argc, char** argv);
    bool Parse();

    void Usage();

    bool SetCount(int count);
    int GetCount() const;

    bool SetSeed(int seed);
    int GetSeed() const;

    bool SetDisplay();
    bool GetDisplay() const;

    bool SetTreeDepth(int depth);
    int GetTreeDepth() const;

    bool SetDealUp();
    bool GetDealUp() const;


private:
    const std::vector<std::string> m_argv;
    mutable int m_seed;
    int m_count;
    int m_treeDepth;
    bool m_display;
    bool m_dealup;
};