using UnrealSharp.Attributes;
using UnrealSharp.Core.Attributes;
using UnrealSharp.Interop;
using UnrealSharp.Core.Marshallers;

namespace UnrealSharp.FashionDragon;

[UClass, GeneratedType("MainCharacter", "UnrealSharp.FashionDragon.MainCharacter")]
public partial class AMainCharacter : UnrealSharp.Engine.ACharacter
{
    static readonly IntPtr NativeClassPtr;
    static AMainCharacter()
    {
        NativeClassPtr = UCoreUObjectExporter.CallGetNativeClassFromName(typeof(AMainCharacter).GetAssemblyName(), "UnrealSharp.FashionDragon", "MainCharacter");
        IntPtr ActiveCamera_NativeProperty = FPropertyExporter.CallGetNativePropertyFromName(NativeClassPtr, "ActiveCamera");
        ActiveCamera_Offset = FPropertyExporter.CallGetPropertyOffset(ActiveCamera_NativeProperty);
        IntPtr FlightHandler_NativeProperty = FPropertyExporter.CallGetNativePropertyFromName(NativeClassPtr, "FlightHandler");
        FlightHandler_Offset = FPropertyExporter.CallGetPropertyOffset(FlightHandler_NativeProperty);
        IntPtr RotationInputHandler_NativeProperty = FPropertyExporter.CallGetNativePropertyFromName(NativeClassPtr, "RotationInputHandler");
        RotationInputHandler_Offset = FPropertyExporter.CallGetPropertyOffset(RotationInputHandler_NativeProperty);
        IntPtr AnimationDebugReporter_NativeProperty = FPropertyExporter.CallGetNativePropertyFromName(NativeClassPtr, "AnimationDebugReporter");
        AnimationDebugReporter_Offset = FPropertyExporter.CallGetPropertyOffset(AnimationDebugReporter_NativeProperty);
        OnLegPlanted_NativeProperty = FPropertyExporter.CallGetNativePropertyFromName(NativeClassPtr, "OnLegPlanted");
        OnLegPlanted_Offset = FPropertyExporter.CallGetPropertyOffset(OnLegPlanted_NativeProperty);
        UnrealSharp.FashionDragon.UOnLegPlanted.InitializeUnrealDelegate(OnLegPlanted_NativeProperty);
        IntPtr DragonMesh_NativeProperty = FPropertyExporter.CallGetNativePropertyFromName(NativeClassPtr, "DragonMesh");
        DragonMesh_Offset = FPropertyExporter.CallGetPropertyOffset(DragonMesh_NativeProperty);
        IntPtr PhysicalAnimation_NativeProperty = FPropertyExporter.CallGetNativePropertyFromName(NativeClassPtr, "PhysicalAnimation");
        PhysicalAnimation_Offset = FPropertyExporter.CallGetPropertyOffset(PhysicalAnimation_NativeProperty);
        IntPtr DragonMeshAsset_NativeProperty = FPropertyExporter.CallGetNativePropertyFromName(NativeClassPtr, "DragonMeshAsset");
        DragonMeshAsset_Offset = FPropertyExporter.CallGetPropertyOffset(DragonMeshAsset_NativeProperty);
        IntPtr MeshRoot_NativeProperty = FPropertyExporter.CallGetNativePropertyFromName(NativeClassPtr, "MeshRoot");
        MeshRoot_Offset = FPropertyExporter.CallGetPropertyOffset(MeshRoot_NativeProperty);
        IntPtr DetachedMeshRoot_NativeProperty = FPropertyExporter.CallGetNativePropertyFromName(NativeClassPtr, "DetachedMeshRoot");
        DetachedMeshRoot_Offset = FPropertyExporter.CallGetPropertyOffset(DetachedMeshRoot_NativeProperty);
        IntPtr SpringArm_NativeProperty = FPropertyExporter.CallGetNativePropertyFromName(NativeClassPtr, "SpringArm");
        SpringArm_Offset = FPropertyExporter.CallGetPropertyOffset(SpringArm_NativeProperty);
        IntPtr MainCamera_NativeProperty = FPropertyExporter.CallGetNativePropertyFromName(NativeClassPtr, "MainCamera");
        MainCamera_Offset = FPropertyExporter.CallGetPropertyOffset(MainCamera_NativeProperty);
        IntPtr AimCamera_NativeProperty = FPropertyExporter.CallGetNativePropertyFromName(NativeClassPtr, "AimCamera");
        AimCamera_Offset = FPropertyExporter.CallGetPropertyOffset(AimCamera_NativeProperty);
    }
    
    static int ActiveCamera_Offset;
    
    public UnrealSharp.Engine.UCameraComponent ActiveCamera
    {
        get
        {
            return ObjectMarshaller<UnrealSharp.Engine.UCameraComponent>.FromNative(IntPtr.Add(NativeObject, ActiveCamera_Offset), 0);
        }
    }
    
    
    static int FlightHandler_Offset;
    
    public UnrealSharp.FashionDragon.UFlightHandler FlightHandler
    {
        get
        {
            return ObjectMarshaller<UnrealSharp.FashionDragon.UFlightHandler>.FromNative(IntPtr.Add(NativeObject, FlightHandler_Offset), 0);
        }
    }
    
    
    static int RotationInputHandler_Offset;
    
    public UnrealSharp.FashionDragon.URotationInputHandler RotationInputHandler
    {
        get
        {
            return ObjectMarshaller<UnrealSharp.FashionDragon.URotationInputHandler>.FromNative(IntPtr.Add(NativeObject, RotationInputHandler_Offset), 0);
        }
    }
    
    
    static int AnimationDebugReporter_Offset;
    
    public UnrealSharp.FashionDragon.UAnimationDebugReporter AnimationDebugReporter
    {
        get
        {
            return ObjectMarshaller<UnrealSharp.FashionDragon.UAnimationDebugReporter>.FromNative(IntPtr.Add(NativeObject, AnimationDebugReporter_Offset), 0);
        }
    }
    
    
    static int OnLegPlanted_Offset;
    static IntPtr OnLegPlanted_NativeProperty;
    private TMulticastDelegate<UnrealSharp.FashionDragon.OnLegPlanted> OnLegPlanted_BackingField;
    
    public TMulticastDelegate<UnrealSharp.FashionDragon.OnLegPlanted> OnLegPlanted
    {
        get
        {
            if (OnLegPlanted_BackingField == null)
            {
                OnLegPlanted_BackingField = MulticastDelegateMarshaller<UnrealSharp.FashionDragon.OnLegPlanted>.FromNative(IntPtr.Add(NativeObject, OnLegPlanted_Offset), OnLegPlanted_NativeProperty, 0);
            }
            return OnLegPlanted_BackingField;
        }
        set
        {
            if (value == OnLegPlanted_BackingField)
            {
                return;
            }
            OnLegPlanted_BackingField = value;
            MulticastDelegateMarshaller<UnrealSharp.FashionDragon.OnLegPlanted>.ToNative(IntPtr.Add(NativeObject, OnLegPlanted_Offset), 0, value);
        }
    }
    
    
    static int DragonMesh_Offset;
    
    public UnrealSharp.Engine.USkeletalMeshComponent DragonMesh
    {
        get
        {
            return ObjectMarshaller<UnrealSharp.Engine.USkeletalMeshComponent>.FromNative(IntPtr.Add(NativeObject, DragonMesh_Offset), 0);
        }
    }
    
    
    static int PhysicalAnimation_Offset;
    
    public UnrealSharp.Engine.UPhysicalAnimationComponent PhysicalAnimation
    {
        get
        {
            return ObjectMarshaller<UnrealSharp.Engine.UPhysicalAnimationComponent>.FromNative(IntPtr.Add(NativeObject, PhysicalAnimation_Offset), 0);
        }
    }
    
    
    static int DragonMeshAsset_Offset;
    
    public UnrealSharp.Engine.USkeletalMesh DragonMeshAsset
    {
        get
        {
            return ObjectMarshaller<UnrealSharp.Engine.USkeletalMesh>.FromNative(IntPtr.Add(NativeObject, DragonMeshAsset_Offset), 0);
        }
    }
    
    
    static int MeshRoot_Offset;
    
    public UnrealSharp.Engine.USceneComponent MeshRoot
    {
        get
        {
            return ObjectMarshaller<UnrealSharp.Engine.USceneComponent>.FromNative(IntPtr.Add(NativeObject, MeshRoot_Offset), 0);
        }
    }
    
    
    static int DetachedMeshRoot_Offset;
    
    public UnrealSharp.Engine.USceneComponent DetachedMeshRoot
    {
        get
        {
            return ObjectMarshaller<UnrealSharp.Engine.USceneComponent>.FromNative(IntPtr.Add(NativeObject, DetachedMeshRoot_Offset), 0);
        }
    }
    
    
    static int SpringArm_Offset;
    
    public UnrealSharp.Engine.USpringArmComponent SpringArm
    {
        get
        {
            return ObjectMarshaller<UnrealSharp.Engine.USpringArmComponent>.FromNative(IntPtr.Add(NativeObject, SpringArm_Offset), 0);
        }
        set
        {
            ObjectMarshaller<UnrealSharp.Engine.USpringArmComponent>.ToNative(IntPtr.Add(NativeObject, SpringArm_Offset), 0, value);
        }
    }
    
    
    static int MainCamera_Offset;
    
    public UnrealSharp.Engine.UCameraComponent MainCamera
    {
        get
        {
            return ObjectMarshaller<UnrealSharp.Engine.UCameraComponent>.FromNative(IntPtr.Add(NativeObject, MainCamera_Offset), 0);
        }
        set
        {
            ObjectMarshaller<UnrealSharp.Engine.UCameraComponent>.ToNative(IntPtr.Add(NativeObject, MainCamera_Offset), 0, value);
        }
    }
    
    
    static int AimCamera_Offset;
    
    public UnrealSharp.Engine.UCameraComponent AimCamera
    {
        get
        {
            return ObjectMarshaller<UnrealSharp.Engine.UCameraComponent>.FromNative(IntPtr.Add(NativeObject, AimCamera_Offset), 0);
        }
        set
        {
            ObjectMarshaller<UnrealSharp.Engine.UCameraComponent>.ToNative(IntPtr.Add(NativeObject, AimCamera_Offset), 0, value);
        }
    }
    
    
}