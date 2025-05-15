#include "Utils.h"

float FUtils::GetRotatorDistance(const FRotator& A)
{
	return GetRotatorDistance(A, FRotator(0, 0, 0));
}

float FUtils::GetRotatorDistance(const FRotator& A, const FRotator& B)
{
	// Convert to quaternions for more accurate interpolation
	const FQuat QuatA = A.Quaternion();
	const FQuat QuatB = B.Quaternion();
    
	// Get the angle between the quaternions in degrees
	return QuatA.AngularDistance(QuatB) * (180.0f / PI);
}
