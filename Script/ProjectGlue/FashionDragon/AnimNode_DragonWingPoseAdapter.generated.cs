using UnrealSharp.Attributes;
using UnrealSharp.Core.Attributes;
using UnrealSharp.Interop;
using UnrealSharp.Core.Marshallers;

namespace UnrealSharp.FashionDragon;

[UStruct, GeneratedType("AnimNode_DragonWingPoseAdapter", "UnrealSharp.FashionDragon.AnimNode_DragonWingPoseAdapter")]
public partial struct FAnimNode_DragonWingPoseAdapter : MarshalledStruct<FAnimNode_DragonWingPoseAdapter>
{
    // InitialUpdateFunction
    
    static int InitialUpdateFunction_Offset;
    /// <summary>
    /// Function called on initial update
    /// </summary>
    private UnrealSharp.Engine.FAnimNodeFunctionRef InitialUpdateFunction;
    
    // BecomeRelevantFunction
    
    static int BecomeRelevantFunction_Offset;
    /// <summary>
    /// Function called on become relevant
    /// </summary>
    private UnrealSharp.Engine.FAnimNodeFunctionRef BecomeRelevantFunction;
    
    // UpdateFunction
    
    static int UpdateFunction_Offset;
    /// <summary>
    /// Function called on update
    /// </summary>
    private UnrealSharp.Engine.FAnimNodeFunctionRef UpdateFunction;
    
    // InputPose
    
    static int InputPose_Offset;
    /// <summary>
    /// Input pose
    /// </summary>
    public UnrealSharp.Engine.FPoseLink InputPose;
    
    // LeftWingEffector
    
    static int LeftWingEffector_Offset;
    public UnrealSharp.FashionDragon.FPoseWingEffector LeftWingEffector;
    
    // RightWingEffector
    
    static int RightWingEffector_Offset;
    public UnrealSharp.FashionDragon.FPoseWingEffector RightWingEffector;
    
    
    static readonly IntPtr NativeClassPtr;
    public static IntPtr GetNativeClassPtr() => NativeClassPtr;
    public static readonly int NativeDataSize;
    public static int GetNativeDataSize() => NativeDataSize;
    static FAnimNode_DragonWingPoseAdapter()
    {
        NativeClassPtr = UCoreUObjectExporter.CallGetNativeStructFromName(typeof(FAnimNode_DragonWingPoseAdapter).GetAssemblyName(), "UnrealSharp.FashionDragon", "AnimNode_DragonWingPoseAdapter");
        #if WITH_EDITOR
        IntPtr InitialUpdateFunction_NativeProperty = FPropertyExporter.CallGetNativePropertyFromName(NativeClassPtr, "InitialUpdateFunction");
        InitialUpdateFunction_Offset = FPropertyExporter.CallGetPropertyOffset(InitialUpdateFunction_NativeProperty);
        #endif
        #if WITH_EDITOR
        IntPtr BecomeRelevantFunction_NativeProperty = FPropertyExporter.CallGetNativePropertyFromName(NativeClassPtr, "BecomeRelevantFunction");
        BecomeRelevantFunction_Offset = FPropertyExporter.CallGetPropertyOffset(BecomeRelevantFunction_NativeProperty);
        #endif
        #if WITH_EDITOR
        IntPtr UpdateFunction_NativeProperty = FPropertyExporter.CallGetNativePropertyFromName(NativeClassPtr, "UpdateFunction");
        UpdateFunction_Offset = FPropertyExporter.CallGetPropertyOffset(UpdateFunction_NativeProperty);
        #endif
        IntPtr InputPose_NativeProperty = FPropertyExporter.CallGetNativePropertyFromName(NativeClassPtr, "InputPose");
        InputPose_Offset = FPropertyExporter.CallGetPropertyOffset(InputPose_NativeProperty);
        IntPtr LeftWingEffector_NativeProperty = FPropertyExporter.CallGetNativePropertyFromName(NativeClassPtr, "LeftWingEffector");
        LeftWingEffector_Offset = FPropertyExporter.CallGetPropertyOffset(LeftWingEffector_NativeProperty);
        IntPtr RightWingEffector_NativeProperty = FPropertyExporter.CallGetNativePropertyFromName(NativeClassPtr, "RightWingEffector");
        RightWingEffector_Offset = FPropertyExporter.CallGetPropertyOffset(RightWingEffector_NativeProperty);
        NativeDataSize = UScriptStructExporter.CallGetNativeStructSize(NativeClassPtr);
    }
    
    
    public FAnimNode_DragonWingPoseAdapter(IntPtr InNativeStruct)
    {
        unsafe
        {
            #if WITH_EDITOR
            InitialUpdateFunction = StructMarshaller<UnrealSharp.Engine.FAnimNodeFunctionRef>.FromNative(IntPtr.Add(InNativeStruct, InitialUpdateFunction_Offset), 0);
            #endif
            #if WITH_EDITOR
            BecomeRelevantFunction = StructMarshaller<UnrealSharp.Engine.FAnimNodeFunctionRef>.FromNative(IntPtr.Add(InNativeStruct, BecomeRelevantFunction_Offset), 0);
            #endif
            #if WITH_EDITOR
            UpdateFunction = StructMarshaller<UnrealSharp.Engine.FAnimNodeFunctionRef>.FromNative(IntPtr.Add(InNativeStruct, UpdateFunction_Offset), 0);
            #endif
            InputPose = StructMarshaller<UnrealSharp.Engine.FPoseLink>.FromNative(IntPtr.Add(InNativeStruct, InputPose_Offset), 0);
            LeftWingEffector = StructMarshaller<UnrealSharp.FashionDragon.FPoseWingEffector>.FromNative(IntPtr.Add(InNativeStruct, LeftWingEffector_Offset), 0);
            RightWingEffector = StructMarshaller<UnrealSharp.FashionDragon.FPoseWingEffector>.FromNative(IntPtr.Add(InNativeStruct, RightWingEffector_Offset), 0);
        }
    }
    
    public static FAnimNode_DragonWingPoseAdapter FromNative(IntPtr buffer) => new FAnimNode_DragonWingPoseAdapter(buffer);
    
    public void ToNative(IntPtr buffer)
    {
        unsafe
        {
            #if WITH_EDITOR
            StructMarshaller<UnrealSharp.Engine.FAnimNodeFunctionRef>.ToNative(IntPtr.Add(buffer, InitialUpdateFunction_Offset), 0, InitialUpdateFunction);
            #endif
            #if WITH_EDITOR
            StructMarshaller<UnrealSharp.Engine.FAnimNodeFunctionRef>.ToNative(IntPtr.Add(buffer, BecomeRelevantFunction_Offset), 0, BecomeRelevantFunction);
            #endif
            #if WITH_EDITOR
            StructMarshaller<UnrealSharp.Engine.FAnimNodeFunctionRef>.ToNative(IntPtr.Add(buffer, UpdateFunction_Offset), 0, UpdateFunction);
            #endif
            StructMarshaller<UnrealSharp.Engine.FPoseLink>.ToNative(IntPtr.Add(buffer, InputPose_Offset), 0, InputPose);
            StructMarshaller<UnrealSharp.FashionDragon.FPoseWingEffector>.ToNative(IntPtr.Add(buffer, LeftWingEffector_Offset), 0, LeftWingEffector);
            StructMarshaller<UnrealSharp.FashionDragon.FPoseWingEffector>.ToNative(IntPtr.Add(buffer, RightWingEffector_Offset), 0, RightWingEffector);
        }
    }
}

public static class FAnimNode_DragonWingPoseAdapterMarshaller
{
    public static FAnimNode_DragonWingPoseAdapter FromNative(IntPtr nativeBuffer, int arrayIndex)
    {
        return new FAnimNode_DragonWingPoseAdapter(nativeBuffer + (arrayIndex * GetNativeDataSize()));
    }
    
    public static void ToNative(IntPtr nativeBuffer, int arrayIndex, FAnimNode_DragonWingPoseAdapter obj)
    {
        obj.ToNative(nativeBuffer + (arrayIndex * GetNativeDataSize()));
    }
    
    public static int GetNativeDataSize()
    {
        return FAnimNode_DragonWingPoseAdapter.NativeDataSize;
    }
}