using UnrealSharp.Attributes;
using UnrealSharp.Core.Attributes;
using UnrealSharp.Interop;
using UnrealSharp.Core.Marshallers;

namespace UnrealSharp.UnrealSharpEditor;

[UEnum, GeneratedType("ECSLoggerVerbosity", "UnrealSharp.UnrealSharpEditor.ECSLoggerVerbosity")]
public enum ECSLoggerVerbosity : byte
{
    /// <summary>
    /// The most minimal output
    /// </summary>
    Quiet = 0,
    /// <summary>
    /// Relatively little output
    /// </summary>
    Minimal = 1,
    /// <summary>
    /// Standard output. This should be the default if verbosity level is not set
    /// </summary>
    Normal = 2,
    /// <summary>
    /// Relatively verbose, but not exhaustive
    /// </summary>
    Detailed = 3,
    /// <summary>
    /// The most verbose and informative verbosity
    /// </summary>
    Diagnostic = 4,
}