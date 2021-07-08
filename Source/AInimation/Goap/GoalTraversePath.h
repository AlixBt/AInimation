#pragma once

#include "CoreMinimal.h"
#include "Goal.h"
#include "../Pathfinding/PathEdge.h"

/**
 * 
 */
class AINIMATION_API GoalTraversePath : public Goal
{
	PathEdge m_pathEdge;
	bool m_bLastEdgeInPath;
	FVector m_vLastNPCPosition;

	bool NPCIsStuck() const;

public:
	GoalTraversePath(AAIIrex* p_pOwner, PathEdge p_pathEdge, bool p_bLastEdge);
	virtual ~GoalTraversePath();

	virtual void Activate() override;
	virtual EStatus Process() override;
	virtual void Terminate() override;
};
