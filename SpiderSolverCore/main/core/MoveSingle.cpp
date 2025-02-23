#include "spidersolvercore/Model/MoveSingle.h"

#include <assert.h>

MoveSingle::MoveSingle()
    : m_fromStack(-1)
    , m_fromIndex(-1)
    , m_destStack(-1)
    , m_destIndex(-1)
{ }

MoveSingle::MoveSingle(int fromStack, int fromIndex, int destStack, int destIndex)
    : m_fromStack(fromStack)
    , m_fromIndex(fromIndex)
    , m_destStack(destStack)
    , m_destIndex(destIndex)
{
    assert(m_fromIndex != -1);
    assert(m_destIndex != -1);

}

MoveSingle::MoveSingle(const MoveSingle& move)
    : m_fromStack(move.m_fromStack)
    , m_fromIndex(move.m_fromIndex)
    , m_destStack(move.m_destStack)
    , m_destIndex(move.m_destIndex)
{
    assert(m_fromIndex != -1);
    assert(m_destIndex != -1);
}

MoveSingle& MoveSingle::operator=(const MoveSingle& move)
{
    m_fromStack = move.m_fromStack;
    m_fromIndex = move.m_fromIndex;
    m_destStack = move.m_destStack;
    m_destIndex = move.m_destIndex;

    assert(m_fromIndex != -1);
    assert(m_destIndex != -1);

    return *this;
}

int MoveSingle::FromStack() const
{
    return m_fromStack;
}

int MoveSingle::FromIndex() const
{
    return m_fromIndex;
}

int MoveSingle::DestStack() const
{
    return m_destStack;
}

int MoveSingle::DestIndex() const
{
    return m_destIndex;
}

bool MoveSingle::operator==(const MoveSingle& rhs)
{
    if (m_fromStack != rhs.m_fromStack)
        return false;
    if (m_fromIndex != rhs.m_fromIndex)
        return false;
    if (m_destStack != rhs.m_destStack)
        return false;
    if (m_destIndex != rhs.m_destIndex)
        return false;

    return true;
}

bool MoveSingle::operator!=(const MoveSingle& rhs)
{
    return !(*this == rhs);
}
