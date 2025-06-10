#include "DragonDriverConstPosition.h"

FPoseEffector FDragonDriverConstPosition::ToEffector(const FPoseEffector& BaseEffector, const FPoseEffectorContext& Context)
{
    if (AdditionMode == EDriverConstPositionMode::Additive)
    {
        return BaseEffector
            .AddPosition(Position)
            .AddRotation(Rotation);
    }
    else if (AdditionMode == EDriverConstPositionMode::Absolute)
    {
        return BaseEffector
            .SetPosition(Position)
            .SetRotation(Rotation);
    }
    return BaseEffector;
}