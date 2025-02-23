#include "CsvTable.h"

#include <iostream>

#include <string>
#include <vector>

namespace
{
    std::string my_to_string(double _Val) { // convert double to string
        const auto _Len = static_cast<size_t>(::_scprintf("%0.3f", _Val));
        std::string _Str(_Len, '\0');
        ::sprintf_s(&_Str[0], _Len + 1, "%0.3f", _Val);
        return _Str;
    }

    std::string my_to_string(float _Val) { // convert float to string
        return my_to_string(static_cast<double>(_Val));
    }

}


void CsvTable::SetHeaders(const std::vector<std::string>& headers)
{
    m_headers = headers;
}


void CsvTable::StartRow()
{
    int rowSize = (int)m_headers.size();
    std::vector<std::string> row(rowSize);
    m_rows.push_back(row);
}


void CsvTable::EndRow()
{

}

void CsvTable::AddStringValue(int columnNumber, const std::string& value)
{
    int last = ((int)m_rows.size()) - 1;
    int width = (int)m_headers.size();

    if (last < 0)
        throw std::exception("null csv table");

    if (columnNumber >= width || columnNumber < 0)
        throw std::exception("bad csv value index.");

    std::vector<std::string>& row = m_rows[last];
    row[columnNumber] = value;
}

void CsvTable::AddValue(int columnNumber, const std::string& value)
{
    AddStringValue(columnNumber, value);
}

void CsvTable::AddValue(int columnNumber, int value)
{
    AddStringValue(columnNumber, std::to_string(value));
}

void CsvTable::AddValue(int columnNumber, double value)
{
    AddStringValue(columnNumber, my_to_string(value));
}

void CsvTable::AddValue(int columnNumber, float value)
{
    AddStringValue(columnNumber, my_to_string(value));
}

void CsvTable::AddValue(int columnNumber, int64_t value)
{
    AddStringValue(columnNumber, std::to_string(value));
}

void CsvTable::AddValue(int columnNumber, bool value)
{
    if (value)
        AddStringValue(columnNumber, "True");
    else
        AddStringValue(columnNumber, "False");
}

void CsvTable::FindMaximumLengths()
{
    int width = (int)m_headers.size();
    m_maxLengths.resize(width);
    for (int column = 0; column < width; column++)
    {
        m_maxLengths[column] = (int)m_headers[column].length();
    }

    for (const auto& row : m_rows)
    {
        for (int column = 0; column < width; column++)
        {
            if (m_maxLengths[column] < row[column].length())
                m_maxLengths[column] = (int)row[column].length();
        }
    }
}

int CsvTable::GetNumberOfRows()
{
    return (int)m_rows.size();
}

std::string spaces(int cnt)
{
    std::string spaces = "";
    for (int i = 0; i < cnt; i++)
        spaces += " ";
    return spaces;
}

void CsvTable::PrintTable()
{
    FindMaximumLengths();

    int width = (int)m_headers.size();

    std::string sep = "";
    for (int col = 0; col < width; col++)
    {
        auto& head = m_headers[col];
        auto sp = spaces(m_maxLengths[col] - (int)head.length());
        std::cout << sep << sp << head;
        sep = ", ";
    }
    std::cout << std::endl;

    for (auto row : m_rows)
    {
        sep = "";
        for (int col = 0; col < width; col++)
        {
            auto& value = row[col];
            auto sp = spaces(m_maxLengths[col] - (int)value.length());
            std::cout << sep << sp << value;
            sep = ", ";
        }
        std::cout << std::endl;
    }
}