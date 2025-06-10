using UnrealSharp.Attributes;
using UnrealSharp.Core.Attributes;
using UnrealSharp.Interop;
using UnrealSharp.Core.Marshallers;

namespace UnrealSharp.FashionDragon;

[UStruct, GeneratedType("PoseWingEffector", "UnrealSharp.FashionDragon.PoseWingEffector")]
public partial struct FPoseWingEffector : MarshalledStruct<FPoseWingEffector>
{
    // Openness
    
    static int Openness_Offset;
    public float Openness;
    
    // FlapAngle
    
    static int FlapAngle_Offset;
    public float FlapAngle;
    
    // TiltAngle
    
    static int TiltAngle_Offset;
    public float TiltAngle;
    
    
    static readonly IntPtr NativeClassPtr;
    public static IntPtr GetNativeClassPtr() => NativeClassPtr;
    public static readonly int NativeDataSize;
    public static int GetNativeDataSize() => NativeDataSize;
    static FPoseWingEffector()
    {
        NativeClassPtr = UCoreUObjectExporter.CallGetNativeStructFromName(typeof(FPoseWingEffector).GetAssemblyName(), "UnrealSharp.FashionDragon", "PoseWingEffector");
        IntPtr Openness_NativeProperty = FPropertyExporter.CallGetNativePropertyFromName(NativeClassPtr, "Openness");
        Openness_Offset = FPropertyExporter.CallGetPropertyOffset(Openness_NativeProperty);
        IntPtr FlapAngle_NativeProperty = FPropertyExporter.CallGetNativePropertyFromName(NativeClassPtr, "FlapAngle");
        FlapAngle_Offset = FPropertyExporter.CallGetPropertyOffset(FlapAngle_NativeProperty);
        IntPtr TiltAngle_NativeProperty = FPropertyExporter.CallGetNativePropertyFromName(NativeClassPtr, "TiltAngle");
        TiltAngle_Offset = FPropertyExporter.CallGetPropertyOffset(TiltAngle_NativeProperty);
        NativeDataSize = UScriptStructExporter.CallGetNativeStructSize(NativeClassPtr);
    }
    
    
    public FPoseWingEffector(IntPtr InNativeStruct)
    {
        unsafe
        {
            Openness = BlittableMarshaller<float>.FromNative(IntPtr.Add(InNativeStruct, Openness_Offset), 0);
            FlapAngle = BlittableMarshaller<float>.FromNative(IntPtr.Add(InNativeStruct, FlapAngle_Offset), 0);
            TiltAngle = BlittableMarshaller<float>.FromNative(IntPtr.Add(InNativeStruct, TiltAngle_Offset), 0);
        }
    }
    
    public static FPoseWingEffector FromNative(IntPtr buffer) => new FPoseWingEffector(buffer);
    
    public void ToNative(IntPtr buffer)
    {
        unsafe
        {
            BlittableMarshaller<float>.ToNative(IntPtr.Add(buffer, Openness_Offset), 0, Openness);
            BlittableMarshaller<float>.ToNative(IntPtr.Add(buffer, FlapAngle_Offset), 0, FlapAngle);
            BlittableMarshaller<float>.ToNative(IntPtr.Add(buffer, TiltAngle_Offset), 0, TiltAngle);
        }
    }
}

public static class FPoseWingEffectorMarshaller
{
    public static FPoseWingEffector FromNative(IntPtr nativeBuffer, int arrayIndex)
    {
        return new FPoseWingEffector(nativeBuffer + (arrayIndex * GetNativeDataSize()));
    }
    
    public static void ToNative(IntPtr nativeBuffer, int arrayIndex, FPoseWingEffector obj)
    {
        obj.ToNative(nativeBuffer + (arrayIndex * GetNativeDataSize()));
    }
    
    public static int GetNativeDataSize()
    {
        return FPoseWingEffector.NativeDataSize;
    }
}