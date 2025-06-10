using UnrealSharp.Attributes;
using UnrealSharp.Core.Attributes;
using UnrealSharp.Interop;
using UnrealSharp.Core.Marshallers;

namespace UnrealSharp.FashionDragon;

[UStruct, GeneratedType("PoseDebugInfo", "UnrealSharp.FashionDragon.PoseDebugInfo")]
public partial struct FPoseDebugInfo : MarshalledStruct<FPoseDebugInfo>
{
    // Name
    
    static int Name_Offset;
    static IntPtr Name_NativeProperty;
    public string Name;
    
    // Drivers
    
    static int Drivers_Offset;
    static IntPtr Drivers_NativeProperty;
    static ArrayCopyMarshaller<UnrealSharp.FashionDragon.FDriverDebugInfo> Drivers_Marshaller = null;
    public System.Collections.Generic.IReadOnlyList<UnrealSharp.FashionDragon.FDriverDebugInfo> Drivers;
    
    
    static readonly IntPtr NativeClassPtr;
    public static IntPtr GetNativeClassPtr() => NativeClassPtr;
    public static readonly int NativeDataSize;
    public static int GetNativeDataSize() => NativeDataSize;
    static FPoseDebugInfo()
    {
        NativeClassPtr = UCoreUObjectExporter.CallGetNativeStructFromName(typeof(FPoseDebugInfo).GetAssemblyName(), "UnrealSharp.FashionDragon", "PoseDebugInfo");
        Name_NativeProperty = FPropertyExporter.CallGetNativePropertyFromName(NativeClassPtr, "Name");
        Name_Offset = FPropertyExporter.CallGetPropertyOffset(Name_NativeProperty);
        Drivers_NativeProperty = FPropertyExporter.CallGetNativePropertyFromName(NativeClassPtr, "Drivers");
        Drivers_Offset = FPropertyExporter.CallGetPropertyOffset(Drivers_NativeProperty);
        NativeDataSize = UScriptStructExporter.CallGetNativeStructSize(NativeClassPtr);
    }
    
    
    public FPoseDebugInfo(IntPtr InNativeStruct)
    {
        unsafe
        {
            IntPtr Name_NativePtr = IntPtr.Add(InNativeStruct,Name_Offset);
            Name = StringMarshaller.FromNative(Name_NativePtr,0);
            Drivers_Marshaller ??= new ArrayCopyMarshaller<UnrealSharp.FashionDragon.FDriverDebugInfo>(Drivers_NativeProperty, StructMarshaller<UnrealSharp.FashionDragon.FDriverDebugInfo>.ToNative, StructMarshaller<UnrealSharp.FashionDragon.FDriverDebugInfo>.FromNative);
            IntPtr Drivers_NativeBuffer = IntPtr.Add(InNativeStruct, Drivers_Offset);
            Drivers = Drivers_Marshaller.FromNative(Drivers_NativeBuffer, 0);
        }
    }
    
    public static FPoseDebugInfo FromNative(IntPtr buffer) => new FPoseDebugInfo(buffer);
    
    public void ToNative(IntPtr buffer)
    {
        unsafe
        {
            IntPtr Name_NativePtr = IntPtr.Add(buffer, Name_Offset);
            StringMarshaller.ToNative(Name_NativePtr,0,Name);
            Drivers_Marshaller ??= new ArrayCopyMarshaller<UnrealSharp.FashionDragon.FDriverDebugInfo>(Drivers_NativeProperty, StructMarshaller<UnrealSharp.FashionDragon.FDriverDebugInfo>.ToNative, StructMarshaller<UnrealSharp.FashionDragon.FDriverDebugInfo>.FromNative);
            IntPtr Drivers_NativeBuffer = IntPtr.Add(buffer, Drivers_Offset);
            Drivers_Marshaller.ToNative(Drivers_NativeBuffer, 0, Drivers);
        }
    }
}

public static class FPoseDebugInfoMarshaller
{
    public static FPoseDebugInfo FromNative(IntPtr nativeBuffer, int arrayIndex)
    {
        return new FPoseDebugInfo(nativeBuffer + (arrayIndex * GetNativeDataSize()));
    }
    
    public static void ToNative(IntPtr nativeBuffer, int arrayIndex, FPoseDebugInfo obj)
    {
        obj.ToNative(nativeBuffer + (arrayIndex * GetNativeDataSize()));
    }
    
    public static int GetNativeDataSize()
    {
        return FPoseDebugInfo.NativeDataSize;
    }
}