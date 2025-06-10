using UnrealSharp.Attributes;
using UnrealSharp.Core.Attributes;
using UnrealSharp.Interop;
using UnrealSharp.Core.Marshallers;

namespace UnrealSharp.FashionDragon;

[UStruct, GeneratedType("DriverDebugInfo", "UnrealSharp.FashionDragon.DriverDebugInfo")]
public partial struct FDriverDebugInfo : MarshalledStruct<FDriverDebugInfo>
{
    // BlendAlpha
    
    static int BlendAlpha_Offset;
    public float BlendAlpha;
    
    // Name
    
    static int Name_Offset;
    static IntPtr Name_NativeProperty;
    public string Name;
    
    // GroupName
    
    static int GroupName_Offset;
    public FName GroupName;
    
    // LayerName
    
    static int LayerName_Offset;
    static IntPtr LayerName_NativeProperty;
    public string LayerName;
    
    // StateInfo
    
    static int StateInfo_Offset;
    static IntPtr StateInfo_NativeProperty;
    public string StateInfo;
    
    // Position
    
    static int Position_Offset;
    public UnrealSharp.CoreUObject.FVector Position;
    
    // Rotation
    
    static int Rotation_Offset;
    public UnrealSharp.CoreUObject.FRotator Rotation;
    
    // PositionDelta
    
    static int PositionDelta_Offset;
    public UnrealSharp.CoreUObject.FVector PositionDelta;
    
    // RotationDelta
    
    static int RotationDelta_Offset;
    public UnrealSharp.CoreUObject.FRotator RotationDelta;
    
    
    static readonly IntPtr NativeClassPtr;
    public static IntPtr GetNativeClassPtr() => NativeClassPtr;
    public static readonly int NativeDataSize;
    public static int GetNativeDataSize() => NativeDataSize;
    static FDriverDebugInfo()
    {
        NativeClassPtr = UCoreUObjectExporter.CallGetNativeStructFromName(typeof(FDriverDebugInfo).GetAssemblyName(), "UnrealSharp.FashionDragon", "DriverDebugInfo");
        IntPtr BlendAlpha_NativeProperty = FPropertyExporter.CallGetNativePropertyFromName(NativeClassPtr, "BlendAlpha");
        BlendAlpha_Offset = FPropertyExporter.CallGetPropertyOffset(BlendAlpha_NativeProperty);
        Name_NativeProperty = FPropertyExporter.CallGetNativePropertyFromName(NativeClassPtr, "Name");
        Name_Offset = FPropertyExporter.CallGetPropertyOffset(Name_NativeProperty);
        IntPtr GroupName_NativeProperty = FPropertyExporter.CallGetNativePropertyFromName(NativeClassPtr, "GroupName");
        GroupName_Offset = FPropertyExporter.CallGetPropertyOffset(GroupName_NativeProperty);
        LayerName_NativeProperty = FPropertyExporter.CallGetNativePropertyFromName(NativeClassPtr, "LayerName");
        LayerName_Offset = FPropertyExporter.CallGetPropertyOffset(LayerName_NativeProperty);
        StateInfo_NativeProperty = FPropertyExporter.CallGetNativePropertyFromName(NativeClassPtr, "StateInfo");
        StateInfo_Offset = FPropertyExporter.CallGetPropertyOffset(StateInfo_NativeProperty);
        IntPtr Position_NativeProperty = FPropertyExporter.CallGetNativePropertyFromName(NativeClassPtr, "Position");
        Position_Offset = FPropertyExporter.CallGetPropertyOffset(Position_NativeProperty);
        IntPtr Rotation_NativeProperty = FPropertyExporter.CallGetNativePropertyFromName(NativeClassPtr, "Rotation");
        Rotation_Offset = FPropertyExporter.CallGetPropertyOffset(Rotation_NativeProperty);
        IntPtr PositionDelta_NativeProperty = FPropertyExporter.CallGetNativePropertyFromName(NativeClassPtr, "PositionDelta");
        PositionDelta_Offset = FPropertyExporter.CallGetPropertyOffset(PositionDelta_NativeProperty);
        IntPtr RotationDelta_NativeProperty = FPropertyExporter.CallGetNativePropertyFromName(NativeClassPtr, "RotationDelta");
        RotationDelta_Offset = FPropertyExporter.CallGetPropertyOffset(RotationDelta_NativeProperty);
        NativeDataSize = UScriptStructExporter.CallGetNativeStructSize(NativeClassPtr);
    }
    
    
    public FDriverDebugInfo(IntPtr InNativeStruct)
    {
        unsafe
        {
            BlendAlpha = BlittableMarshaller<float>.FromNative(IntPtr.Add(InNativeStruct, BlendAlpha_Offset), 0);
            IntPtr Name_NativePtr = IntPtr.Add(InNativeStruct,Name_Offset);
            Name = StringMarshaller.FromNative(Name_NativePtr,0);
            GroupName = BlittableMarshaller<FName>.FromNative(IntPtr.Add(InNativeStruct, GroupName_Offset), 0);
            IntPtr LayerName_NativePtr = IntPtr.Add(InNativeStruct,LayerName_Offset);
            LayerName = StringMarshaller.FromNative(LayerName_NativePtr,0);
            IntPtr StateInfo_NativePtr = IntPtr.Add(InNativeStruct,StateInfo_Offset);
            StateInfo = StringMarshaller.FromNative(StateInfo_NativePtr,0);
            Position = BlittableMarshaller<UnrealSharp.CoreUObject.FVector>.FromNative(IntPtr.Add(InNativeStruct, Position_Offset), 0);
            Rotation = BlittableMarshaller<UnrealSharp.CoreUObject.FRotator>.FromNative(IntPtr.Add(InNativeStruct, Rotation_Offset), 0);
            PositionDelta = BlittableMarshaller<UnrealSharp.CoreUObject.FVector>.FromNative(IntPtr.Add(InNativeStruct, PositionDelta_Offset), 0);
            RotationDelta = BlittableMarshaller<UnrealSharp.CoreUObject.FRotator>.FromNative(IntPtr.Add(InNativeStruct, RotationDelta_Offset), 0);
        }
    }
    
    public static FDriverDebugInfo FromNative(IntPtr buffer) => new FDriverDebugInfo(buffer);
    
    public void ToNative(IntPtr buffer)
    {
        unsafe
        {
            BlittableMarshaller<float>.ToNative(IntPtr.Add(buffer, BlendAlpha_Offset), 0, BlendAlpha);
            IntPtr Name_NativePtr = IntPtr.Add(buffer, Name_Offset);
            StringMarshaller.ToNative(Name_NativePtr,0,Name);
            BlittableMarshaller<FName>.ToNative(IntPtr.Add(buffer, GroupName_Offset), 0, GroupName);
            IntPtr LayerName_NativePtr = IntPtr.Add(buffer, LayerName_Offset);
            StringMarshaller.ToNative(LayerName_NativePtr,0,LayerName);
            IntPtr StateInfo_NativePtr = IntPtr.Add(buffer, StateInfo_Offset);
            StringMarshaller.ToNative(StateInfo_NativePtr,0,StateInfo);
            BlittableMarshaller<UnrealSharp.CoreUObject.FVector>.ToNative(IntPtr.Add(buffer, Position_Offset), 0, Position);
            BlittableMarshaller<UnrealSharp.CoreUObject.FRotator>.ToNative(IntPtr.Add(buffer, Rotation_Offset), 0, Rotation);
            BlittableMarshaller<UnrealSharp.CoreUObject.FVector>.ToNative(IntPtr.Add(buffer, PositionDelta_Offset), 0, PositionDelta);
            BlittableMarshaller<UnrealSharp.CoreUObject.FRotator>.ToNative(IntPtr.Add(buffer, RotationDelta_Offset), 0, RotationDelta);
        }
    }
}

public static class FDriverDebugInfoMarshaller
{
    public static FDriverDebugInfo FromNative(IntPtr nativeBuffer, int arrayIndex)
    {
        return new FDriverDebugInfo(nativeBuffer + (arrayIndex * GetNativeDataSize()));
    }
    
    public static void ToNative(IntPtr nativeBuffer, int arrayIndex, FDriverDebugInfo obj)
    {
        obj.ToNative(nativeBuffer + (arrayIndex * GetNativeDataSize()));
    }
    
    public static int GetNativeDataSize()
    {
        return FDriverDebugInfo.NativeDataSize;
    }
}