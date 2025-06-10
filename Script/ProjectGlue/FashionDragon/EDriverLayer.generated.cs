using UnrealSharp.Attributes;
using UnrealSharp.Core.Attributes;
using UnrealSharp.Interop;
using UnrealSharp.Core.Marshallers;

namespace UnrealSharp.FashionDragon;

/// <summary>
/// All layers are fully virtualized, and their output is summed
/// </summary>
[UEnum, GeneratedType("EDriverLayer", "UnrealSharp.FashionDragon.EDriverLayer")]
public enum EDriverLayer : byte
{
    /// <summary>
    /// Main movement drivers (idle, walk, jump for legs)
    /// </summary>
    Primary = 0,
    /// <summary>
    /// Independent movement components
    /// </summary>
    Constant = 1,
    Jump = 2,
    Sway = 3,
    Momentum = 4,
    RotateToMovement = 5,
    /// <summary>
    /// Special - do not use
    /// </summary>
    TotalCount,
    Unknown,
    Output,
}