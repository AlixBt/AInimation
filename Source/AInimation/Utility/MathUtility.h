#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
class AINIMATION_API MathUtility
{
public:
	MathUtility();
	~MathUtility();

	static FVector getOrthogonal2D(FVector t_source);
};
