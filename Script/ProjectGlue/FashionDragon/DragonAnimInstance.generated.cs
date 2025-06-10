using UnrealSharp.Attributes;
using UnrealSharp.Core.Attributes;
using UnrealSharp.Interop;
using UnrealSharp.Core.Marshallers;

namespace UnrealSharp.FashionDragon;

[UClass, GeneratedType("DragonAnimInstance", "UnrealSharp.FashionDragon.DragonAnimInstance")]
public partial class UDragonAnimInstance : UnrealSharp.Engine.UAnimInstance
{
    static readonly IntPtr NativeClassPtr;
    static UDragonAnimInstance()
    {
        NativeClassPtr = UCoreUObjectExporter.CallGetNativeClassFromName(typeof(UDragonAnimInstance).GetAssemblyName(), "UnrealSharp.FashionDragon", "DragonAnimInstance");
        IntPtr HeadTransform_NativeProperty = FPropertyExporter.CallGetNativePropertyFromName(NativeClassPtr, "HeadTransform");
        HeadTransform_Offset = FPropertyExporter.CallGetPropertyOffset(HeadTransform_NativeProperty);
        IntPtr UpperNeckTransform_NativeProperty = FPropertyExporter.CallGetNativePropertyFromName(NativeClassPtr, "UpperNeckTransform");
        UpperNeckTransform_Offset = FPropertyExporter.CallGetPropertyOffset(UpperNeckTransform_NativeProperty);
        IntPtr LowerNeckTransform_NativeProperty = FPropertyExporter.CallGetNativePropertyFromName(NativeClassPtr, "LowerNeckTransform");
        LowerNeckTransform_Offset = FPropertyExporter.CallGetPropertyOffset(LowerNeckTransform_NativeProperty);
        IntPtr SpineFrontTransform_NativeProperty = FPropertyExporter.CallGetNativePropertyFromName(NativeClassPtr, "SpineFrontTransform");
        SpineFrontTransform_Offset = FPropertyExporter.CallGetPropertyOffset(SpineFrontTransform_NativeProperty);
        IntPtr SpineBackTransform_NativeProperty = FPropertyExporter.CallGetNativePropertyFromName(NativeClassPtr, "SpineBackTransform");
        SpineBackTransform_Offset = FPropertyExporter.CallGetPropertyOffset(SpineBackTransform_NativeProperty);
        IntPtr HipTransform_NativeProperty = FPropertyExporter.CallGetNativePropertyFromName(NativeClassPtr, "HipTransform");
        HipTransform_Offset = FPropertyExporter.CallGetPropertyOffset(HipTransform_NativeProperty);
        HandTransforms_NativeProperty = FPropertyExporter.CallGetNativePropertyFromName(NativeClassPtr, "HandTransforms");
        HandTransforms_Offset = FPropertyExporter.CallGetPropertyOffset(HandTransforms_NativeProperty);
        LegPositions_NativeProperty = FPropertyExporter.CallGetNativePropertyFromName(NativeClassPtr, "LegPositions");
        LegPositions_Offset = FPropertyExporter.CallGetPropertyOffset(LegPositions_NativeProperty);
        LegRotations_NativeProperty = FPropertyExporter.CallGetNativePropertyFromName(NativeClassPtr, "LegRotations");
        LegRotations_Offset = FPropertyExporter.CallGetPropertyOffset(LegRotations_NativeProperty);
        WingEffectors_NativeProperty = FPropertyExporter.CallGetNativePropertyFromName(NativeClassPtr, "WingEffectors");
        WingEffectors_Offset = FPropertyExporter.CallGetPropertyOffset(WingEffectors_NativeProperty);
    }
    
    static int HeadTransform_Offset;
    
    /// <summary>
    /// IK Rig outputs
    /// </summary>
    public UnrealSharp.CoreUObject.FTransform HeadTransform
    {
        get
        {
            return BlittableMarshaller<UnrealSharp.CoreUObject.FTransform>.FromNative(IntPtr.Add(NativeObject, HeadTransform_Offset), 0);
        }
        set
        {
            BlittableMarshaller<UnrealSharp.CoreUObject.FTransform>.ToNative(IntPtr.Add(NativeObject, HeadTransform_Offset), 0, value);
        }
    }
    
    
    static int UpperNeckTransform_Offset;
    
    public UnrealSharp.CoreUObject.FTransform UpperNeckTransform
    {
        get
        {
            return BlittableMarshaller<UnrealSharp.CoreUObject.FTransform>.FromNative(IntPtr.Add(NativeObject, UpperNeckTransform_Offset), 0);
        }
        set
        {
            BlittableMarshaller<UnrealSharp.CoreUObject.FTransform>.ToNative(IntPtr.Add(NativeObject, UpperNeckTransform_Offset), 0, value);
        }
    }
    
    
    static int LowerNeckTransform_Offset;
    
    public UnrealSharp.CoreUObject.FTransform LowerNeckTransform
    {
        get
        {
            return BlittableMarshaller<UnrealSharp.CoreUObject.FTransform>.FromNative(IntPtr.Add(NativeObject, LowerNeckTransform_Offset), 0);
        }
        set
        {
            BlittableMarshaller<UnrealSharp.CoreUObject.FTransform>.ToNative(IntPtr.Add(NativeObject, LowerNeckTransform_Offset), 0, value);
        }
    }
    
    
    static int SpineFrontTransform_Offset;
    
    public UnrealSharp.CoreUObject.FTransform SpineFrontTransform
    {
        get
        {
            return BlittableMarshaller<UnrealSharp.CoreUObject.FTransform>.FromNative(IntPtr.Add(NativeObject, SpineFrontTransform_Offset), 0);
        }
        set
        {
            BlittableMarshaller<UnrealSharp.CoreUObject.FTransform>.ToNative(IntPtr.Add(NativeObject, SpineFrontTransform_Offset), 0, value);
        }
    }
    
    
    static int SpineBackTransform_Offset;
    
    public UnrealSharp.CoreUObject.FTransform SpineBackTransform
    {
        get
        {
            return BlittableMarshaller<UnrealSharp.CoreUObject.FTransform>.FromNative(IntPtr.Add(NativeObject, SpineBackTransform_Offset), 0);
        }
        set
        {
            BlittableMarshaller<UnrealSharp.CoreUObject.FTransform>.ToNative(IntPtr.Add(NativeObject, SpineBackTransform_Offset), 0, value);
        }
    }
    
    
    static int HipTransform_Offset;
    
    public UnrealSharp.CoreUObject.FTransform HipTransform
    {
        get
        {
            return BlittableMarshaller<UnrealSharp.CoreUObject.FTransform>.FromNative(IntPtr.Add(NativeObject, HipTransform_Offset), 0);
        }
        set
        {
            BlittableMarshaller<UnrealSharp.CoreUObject.FTransform>.ToNative(IntPtr.Add(NativeObject, HipTransform_Offset), 0, value);
        }
    }
    
    
    static int HandTransforms_Offset;
    static IntPtr HandTransforms_NativeProperty;
    ArrayMarshaller<UnrealSharp.CoreUObject.FTransform> HandTransforms_Marshaller = null;
    
    public System.Collections.Generic.IList<UnrealSharp.CoreUObject.FTransform> HandTransforms
    {
        get
        {
            HandTransforms_Marshaller ??= new ArrayMarshaller<UnrealSharp.CoreUObject.FTransform>(HandTransforms_NativeProperty, BlittableMarshaller<UnrealSharp.CoreUObject.FTransform>.ToNative, BlittableMarshaller<UnrealSharp.CoreUObject.FTransform>.FromNative);
            return HandTransforms_Marshaller.FromNative(IntPtr.Add(NativeObject, HandTransforms_Offset), 0);
        }
    }
    
    
    static int LegPositions_Offset;
    static IntPtr LegPositions_NativeProperty;
    ArrayMarshaller<UnrealSharp.CoreUObject.FVector> LegPositions_Marshaller = null;
    
    public System.Collections.Generic.IList<UnrealSharp.CoreUObject.FVector> LegPositions
    {
        get
        {
            LegPositions_Marshaller ??= new ArrayMarshaller<UnrealSharp.CoreUObject.FVector>(LegPositions_NativeProperty, BlittableMarshaller<UnrealSharp.CoreUObject.FVector>.ToNative, BlittableMarshaller<UnrealSharp.CoreUObject.FVector>.FromNative);
            return LegPositions_Marshaller.FromNative(IntPtr.Add(NativeObject, LegPositions_Offset), 0);
        }
    }
    
    
    static int LegRotations_Offset;
    static IntPtr LegRotations_NativeProperty;
    ArrayMarshaller<UnrealSharp.CoreUObject.FRotator> LegRotations_Marshaller = null;
    
    public System.Collections.Generic.IList<UnrealSharp.CoreUObject.FRotator> LegRotations
    {
        get
        {
            LegRotations_Marshaller ??= new ArrayMarshaller<UnrealSharp.CoreUObject.FRotator>(LegRotations_NativeProperty, BlittableMarshaller<UnrealSharp.CoreUObject.FRotator>.ToNative, BlittableMarshaller<UnrealSharp.CoreUObject.FRotator>.FromNative);
            return LegRotations_Marshaller.FromNative(IntPtr.Add(NativeObject, LegRotations_Offset), 0);
        }
    }
    
    
    static int WingEffectors_Offset;
    static IntPtr WingEffectors_NativeProperty;
    ArrayMarshaller<UnrealSharp.FashionDragon.FPoseWingEffector> WingEffectors_Marshaller = null;
    
    public System.Collections.Generic.IList<UnrealSharp.FashionDragon.FPoseWingEffector> WingEffectors
    {
        get
        {
            WingEffectors_Marshaller ??= new ArrayMarshaller<UnrealSharp.FashionDragon.FPoseWingEffector>(WingEffectors_NativeProperty, StructMarshaller<UnrealSharp.FashionDragon.FPoseWingEffector>.ToNative, StructMarshaller<UnrealSharp.FashionDragon.FPoseWingEffector>.FromNative);
            return WingEffectors_Marshaller.FromNative(IntPtr.Add(NativeObject, WingEffectors_Offset), 0);
        }
    }
    
    
}