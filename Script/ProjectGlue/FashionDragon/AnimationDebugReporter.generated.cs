using UnrealSharp.Attributes;
using UnrealSharp.Core.Attributes;
using UnrealSharp.Interop;
using UnrealSharp.Core.Marshallers;

namespace UnrealSharp.FashionDragon;

[UClass, GeneratedType("AnimationDebugReporter", "UnrealSharp.FashionDragon.AnimationDebugReporter")]
public partial class UAnimationDebugReporter : UnrealSharp.FashionDragon.UPlayerComponent
{
    static readonly IntPtr NativeClassPtr;
    static UAnimationDebugReporter()
    {
        NativeClassPtr = UCoreUObjectExporter.CallGetNativeClassFromName(typeof(UAnimationDebugReporter).GetAssemblyName(), "UnrealSharp.FashionDragon", "AnimationDebugReporter");
        PoseDebugInfo_NativeProperty = FPropertyExporter.CallGetNativePropertyFromName(NativeClassPtr, "PoseDebugInfo");
        PoseDebugInfo_Offset = FPropertyExporter.CallGetPropertyOffset(PoseDebugInfo_NativeProperty);
        SetPoseEnabled_NativeFunction = UClassExporter.CallGetNativeFunctionFromClassAndName(NativeClassPtr, "SetPoseEnabled");
        SetPoseEnabled_ParamsSize = UFunctionExporter.CallGetNativeFunctionParamsSize(SetPoseEnabled_NativeFunction);
        SetPoseEnabled_PoseIndex_Offset = FPropertyExporter.CallGetPropertyOffsetFromName(SetPoseEnabled_NativeFunction, "PoseIndex");
        SetPoseEnabled_Enabled_Offset = FPropertyExporter.CallGetPropertyOffsetFromName(SetPoseEnabled_NativeFunction, "Enabled");
    }
    
    static int PoseDebugInfo_Offset;
    static IntPtr PoseDebugInfo_NativeProperty;
    ArrayReadOnlyMarshaller<UnrealSharp.FashionDragon.FPoseDebugInfo> PoseDebugInfo_Marshaller = null;
    
    public System.Collections.Generic.IReadOnlyList<UnrealSharp.FashionDragon.FPoseDebugInfo> PoseDebugInfo
    {
        get
        {
            PoseDebugInfo_Marshaller ??= new ArrayReadOnlyMarshaller<UnrealSharp.FashionDragon.FPoseDebugInfo>(PoseDebugInfo_NativeProperty, StructMarshaller<UnrealSharp.FashionDragon.FPoseDebugInfo>.ToNative, StructMarshaller<UnrealSharp.FashionDragon.FPoseDebugInfo>.FromNative);
            return PoseDebugInfo_Marshaller.FromNative(IntPtr.Add(NativeObject, PoseDebugInfo_Offset), 0);
        }
    }
    
    // SetPoseEnabled
    static IntPtr SetPoseEnabled_NativeFunction;
    static int SetPoseEnabled_ParamsSize;
    static int SetPoseEnabled_PoseIndex_Offset;
    static int SetPoseEnabled_Enabled_Offset;
    
    [UFunction, GeneratedType("SetPoseEnabled")]
    public void SetPoseEnabled(int poseIndex, bool enabled)
    {
        unsafe
        {
            byte* paramsBufferAllocation = stackalloc byte[SetPoseEnabled_ParamsSize];
            nint paramsBuffer = (nint) paramsBufferAllocation;
            BlittableMarshaller<int>.ToNative(IntPtr.Add(paramsBuffer, SetPoseEnabled_PoseIndex_Offset), 0, poseIndex);
            BoolMarshaller.ToNative(IntPtr.Add(paramsBuffer, SetPoseEnabled_Enabled_Offset), 0, enabled);
            
            UObjectExporter.CallInvokeNativeFunction(NativeObject, SetPoseEnabled_NativeFunction, paramsBuffer);
            
        }
    }
    
    
}