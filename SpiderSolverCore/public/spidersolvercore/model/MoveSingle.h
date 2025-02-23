#pragma once

#include "spidersolvercore/Model/Card.h"

#include <array>


class MoveSingle
{
public:
    MoveSingle();
    MoveSingle(int fromStack, int fromIndex, int destStack, int destIndex);
    MoveSingle(const MoveSingle& move);
    MoveSingle& operator=(const MoveSingle& move);

public:
    int FromStack() const;
    int FromIndex() const;
    int DestStack() const;
    int DestIndex() const;

    bool operator==(const MoveSingle& rhs);
    bool operator!=(const MoveSingle& rhs);

private:
    int m_fromStack;
    int m_fromIndex;
    int m_destStack;
    int m_destIndex;
};
