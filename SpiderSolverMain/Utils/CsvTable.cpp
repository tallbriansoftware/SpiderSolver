#include "CsvTable.h"

#include <iostream>


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

    std::string spaces(int cnt)
    {
        std::string spaces = "";
        for (int i = 0; i < cnt; i++)
            spaces += " ";
        return spaces;
    }
}


CsvTable::CsvTable(const std::vector<CsvTable::Column>& columns)
{
    m_columns = columns;
}

void CsvTable::AddColumn(std::string name, Types type)
{
    if (m_rows.size() != 0)
        throw("cannot add columns to a table with nonzero rows");

    m_columns.push_back({name, type});
}


void CsvTable::StartRow()
{
    int rowSize = (int)m_columns.size();
    std::vector<std::string> row(rowSize);
    m_rows.push_back(row);
}

void CsvTable::EndRow()
{

}

int CsvTable::GetColumnIndex(const std::string& header)
{
    int idx = 0;
    for (auto& col : m_columns)
    {
        if (col.header == header)
            return idx;
        idx += 1;
    }
    throw std::exception("bad column name");
}

bool CsvTable::CheckType(int columnIndex, Types type)
{
    if (m_columns[columnIndex].type != type)
        throw std::exception("bad column type");
    return true;
}

void CsvTable::AddStringValue(int columnIndex, const std::string& value)
{
    int last = ((int)m_rows.size()) - 1;
    int width = (int)m_columns.size();

    if (last < 0)
        throw std::exception("null csv table");

    if (columnIndex >= width || columnIndex < 0)
        throw std::exception("bad csv value index.");

    std::vector<std::string>& row = m_rows[last];
    row[columnIndex] = value;
}

void CsvTable::AddValue(const std::string& header, const std::string& value)
{
    int columnIndex = GetColumnIndex(header);
    CheckType(columnIndex, Types::typeString);
    AddStringValue(columnIndex, value);
}

void CsvTable::AddValue(const std::string& header, int value)
{
    int columnIndex = GetColumnIndex(header);
    CheckType(columnIndex, Types::typeInt);
    AddStringValue(columnIndex, std::to_string(value));
}

void CsvTable::AddValue(const std::string& header, double value)
{
    int columnIndex = GetColumnIndex(header);
    CheckType(columnIndex, Types::typeDouble);
    AddStringValue(columnIndex, my_to_string(value));

}

void CsvTable::AddValue(const std::string& header, float value)
{
    int columnIndex = GetColumnIndex(header);
    CheckType(columnIndex, Types::typeFloat);
    AddStringValue(columnIndex, my_to_string(value));
}

void CsvTable::AddValue(const std::string& header, int64_t value)
{
    int columnIndex = GetColumnIndex(header);
    CheckType(columnIndex, Types::typeInt64);
    AddStringValue(columnIndex, std::to_string(value));
}

void CsvTable::AddValue(const std::string& header, bool value)
{
    int columnIndex = GetColumnIndex(header);
    CheckType(columnIndex, Types::typeBool);
    if (value)
        AddStringValue(columnIndex, "True");
    else
        AddStringValue(columnIndex, "False");
}

void CsvTable::FindMaximumLengths()
{
    int width = (int)m_columns.size();
    m_maxLengths.resize(width);
    for (int column = 0; column < width; column++)
    {
        m_maxLengths[column] = (int)m_columns[column].header.length();
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

void CsvTable::PrintTable()
{
    FindMaximumLengths();

    int width = (int)m_columns.size();

    std::string sep = "";
    for (int col = 0; col < width; col++)
    {
        auto& head = m_columns[col].header;
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