using UnrealSharp.Attributes;
using UnrealSharp.Core.Attributes;
using UnrealSharp.Interop;
using UnrealSharp.Core.Marshallers;

namespace UnrealSharp.FashionDragonEditor;

[UClass, GeneratedType("AnimGraphNode_DragonWingPoseAdapter", "UnrealSharp.FashionDragonEditor.AnimGraphNode_DragonWingPoseAdapter")]
public partial class UAnimGraphNode_DragonWingPoseAdapter : UnrealSharp.AnimGraph.UAnimGraphNode_Base
{
    static readonly IntPtr NativeClassPtr;
    static UAnimGraphNode_DragonWingPoseAdapter()
    {
        NativeClassPtr = UCoreUObjectExporter.CallGetNativeClassFromName(typeof(UAnimGraphNode_DragonWingPoseAdapter).GetAssemblyName(), "UnrealSharp.FashionDragonEditor", "AnimGraphNode_DragonWingPoseAdapter");
        IntPtr Node_NativeProperty = FPropertyExporter.CallGetNativePropertyFromName(NativeClassPtr, "Node");
        Node_Offset = FPropertyExporter.CallGetPropertyOffset(Node_NativeProperty);
    }
    
    static int Node_Offset;
    
    public UnrealSharp.FashionDragon.FAnimNode_DragonWingPoseAdapter Node
    {
        get
        {
            return StructMarshaller<UnrealSharp.FashionDragon.FAnimNode_DragonWingPoseAdapter>.FromNative(IntPtr.Add(NativeObject, Node_Offset), 0);
        }
    }
    
    
}