#include "spidersolvercore/Model/SpiderNode.h"

#include "spidersolvercore/model/SpiderTableau.h"
#include "spidersolvercore/strategy/BoardScorer.h"
#include "spidersolvercore/model/SpiderString.h"



SpiderNode::SpiderNode()
    : m_depth(-1)
{ }


SpiderNode::SpiderNode(
    int depth,
    const std::string& tableauString,
    float score)
    : m_depth(depth)
    , m_stringRep(tableauString)
    , m_score(score)
{ 
}

int SpiderNode::GetDepth() const
{
    return m_depth;
}


float SpiderNode::GetScore() const
{
    return m_score;
}

const std::string& SpiderNode::GetStringRep() const
{
    return m_stringRep;
}
