using UnrealSharp.Attributes;
using UnrealSharp.Core.Attributes;
using UnrealSharp.Interop;
using UnrealSharp.Core.Marshallers;

namespace UnrealSharp.FashionDragon;

[UEnum, GeneratedType("ELegIdleState", "UnrealSharp.FashionDragon.ELegIdleState")]
public enum ELegIdleState
{
    Relaxed = 0,
    NeedsReturn = 1,
    Planted = 2,
    ArticulatedReturn = 3,
}