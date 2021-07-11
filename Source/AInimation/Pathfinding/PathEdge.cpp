#include "PathEdge.h"

PathEdge::PathEdge(FVector p_vSource, FVector p_vDestination, EBehaviorType p_eBehavior) :
	m_vSourcePosition(p_vSource),
	m_vDestinationPosition(p_vDestination),
	m_eBehavior(p_eBehavior)
{
}

PathEdge::~PathEdge()
{
}

FVector PathEdge::GetSourcePosition() const
{
	return m_vSourcePosition;
}

FVector PathEdge::GetDestinationPosition() const
{
	return m_vDestinationPosition;
}

EBehaviorType PathEdge::GetBehaviorType() const
{
	return m_eBehavior;
}

void PathEdge::SetSourcePosition(FVector p_vSourcePosition)
{
	m_vSourcePosition = p_vSourcePosition;
}

void PathEdge::SetDestinationPosition(FVector p_vDestinationPosition)
{
	m_vDestinationPosition = p_vDestinationPosition;
}

void PathEdge::setNextSourcePosition(FVector t_nextSourcePosition)
{
	m_nextSourcePosition = t_nextSourcePosition;
}

void PathEdge::setFirstControlPoint(FVector t_firstControlPoint)
{
	m_firstControlPoint = t_firstControlPoint;
}

void PathEdge::setSecondControlPoint(FVector t_secondControlPoint)
{
	m_secondControlPoint = t_secondControlPoint;
}
