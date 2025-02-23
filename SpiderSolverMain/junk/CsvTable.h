#pragma once

#include <string>
#include <vector>


class CsvTable
{
public:
    void SetHeaders(const std::vector<std::string>& headerStrings);

    void StartRow();
    void EndRow();

    void AddValue(int columnNumber, const std::string& value);

    void AddValue(int columnNumber, int value);
    void AddValue(int columnNumber, double value);
    void AddValue(int columnNumber, float value);
    void AddValue(int columnNumber, int64_t value);
    void AddValue(int columnNumber, bool value);


    int GetNumberOfRows();
    void PrintTable();

private:
    void AddStringValue(int index, const std::string& value);
    void FindMaximumLengths();


private:
    std::vector<std::string> m_headers;
    std::vector<int> m_maxLengths;
    std::vector<std::vector<std::string>> m_rows;
};