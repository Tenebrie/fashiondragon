using UnrealSharp.Attributes;
using UnrealSharp.Core.Attributes;
using UnrealSharp.Interop;
using UnrealSharp.Core.Marshallers;

namespace UnrealSharp.FashionDragon;

[UEnum, GeneratedType("ELegWalkingState", "UnrealSharp.FashionDragon.ELegWalkingState")]
public enum ELegWalkingState
{
    Relaxed = 0,
    Raised = 1,
    Stepping = 2,
    /// <summary>
    /// The leg is moving straight down to the ground
    /// </summary>
    SeekingGround = 3,
    /// <summary>
    /// The leg's position is locked in world space
    /// </summary>
    Planted = 4,
    Inertia = 5,
}