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

FVector MathUtility::getDotProductVector2D(FVector t_source, float t_dotProduct)
{
    return FVector(1, t_dotProduct - t_source.X / t_source.Y, 0.0f);
}

FVector MathUtility::getCrossProductVector2D(FVector t_source, float t_crossProduct)
{
    return FVector(t_crossProduct - t_source.X / t_source.Y, 1, 0.0f);
}

