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
	static FVector getDotProductVector2D(FVector t_source, float t_dotProduct);
	static FVector getCrossProductVector2D(FVector t_source, float t_crossProduct);
};
