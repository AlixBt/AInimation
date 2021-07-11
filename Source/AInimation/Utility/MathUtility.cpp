#include "MathUtility.h"

MathUtility::MathUtility()
{
}

MathUtility::~MathUtility()
{
}

FVector MathUtility::getOrthogonal2D(FVector t_source)
{
    // We choose an arbitrary value (1) for the X component and we solve for Y
    return FVector(1, -t_source.X / t_source.Y, 0.0f);
}
