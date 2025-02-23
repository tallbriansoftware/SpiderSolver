#pragma once

#include <string>
#include <vector>



class CsvTable
{
public:
    enum Types { typeInt, typeString, typeDouble, typeFloat, typeInt64, typeBool };

    struct Column {
        std::string header;
        Types type;
    };

public:
    CsvTable(const std::vector<Column>& columns);

    void AddColumn(std::string name, Types type);

    void StartRow();
    void EndRow();

    void AddValue(const std::string& header, const std::string& value);

    void AddValue(const std::string& header, int value);
    void AddValue(const std::string& header, double value);
    void AddValue(const std::string& header, float value);
    void AddValue(const std::string& header, int64_t value);
    void AddValue(const std::string& header, bool value);


    int GetNumberOfRows();
    void PrintTable();

private:
    void AddStringValue(int index, const std::string& value);
    void FindMaximumLengths();
    int GetColumnIndex(const std::string& header);
    bool CheckType(int columnIndex, Types type);

private:
    std::vector<Column> m_columns;
    std::vector<int> m_maxLengths;
    std::vector<std::vector<std::string>> m_rows;
};