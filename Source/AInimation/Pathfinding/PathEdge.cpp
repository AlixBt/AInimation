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

FVector PathEdge::getSourceDirection() const
{
	return m_sourceDirection;
}

FVector PathEdge::getTargetDirection() const
{
	return m_targetDirection;
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

void PathEdge::setSourceDirection(FVector t_sourceDirection)
{
	m_sourceDirection = t_sourceDirection;
}

void PathEdge::setTargetDirection(FVector t_targetDirection)
{
	m_targetDirection = t_targetDirection;
}
