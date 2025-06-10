using UnrealSharp.Attributes;
using UnrealSharp.Core.Attributes;
using UnrealSharp.Interop;
using UnrealSharp.Core.Marshallers;

namespace UnrealSharp.FashionDragon;

[UClass, GeneratedType("DefaultPlayerController", "UnrealSharp.FashionDragon.DefaultPlayerController")]
public partial class ADefaultPlayerController : UnrealSharp.FashionDragon.ABasePlayerController
{
    static readonly IntPtr NativeClassPtr;
    static ADefaultPlayerController()
    {
        NativeClassPtr = UCoreUObjectExporter.CallGetNativeClassFromName(typeof(ADefaultPlayerController).GetAssemblyName(), "UnrealSharp.FashionDragon", "DefaultPlayerController");
        IntPtr InputSubsystem_NativeProperty = FPropertyExporter.CallGetNativePropertyFromName(NativeClassPtr, "InputSubsystem");
        InputSubsystem_Offset = FPropertyExporter.CallGetPropertyOffset(InputSubsystem_NativeProperty);
        IntPtr CommonInputContext_NativeProperty = FPropertyExporter.CallGetNativePropertyFromName(NativeClassPtr, "CommonInputContext");
        CommonInputContext_Offset = FPropertyExporter.CallGetPropertyOffset(CommonInputContext_NativeProperty);
        IntPtr DragonGroundInputContext_NativeProperty = FPropertyExporter.CallGetNativePropertyFromName(NativeClassPtr, "DragonGroundInputContext");
        DragonGroundInputContext_Offset = FPropertyExporter.CallGetPropertyOffset(DragonGroundInputContext_NativeProperty);
        IntPtr DragonFlyingInputContext_NativeProperty = FPropertyExporter.CallGetNativePropertyFromName(NativeClassPtr, "DragonFlyingInputContext");
        DragonFlyingInputContext_Offset = FPropertyExporter.CallGetPropertyOffset(DragonFlyingInputContext_NativeProperty);
        IntPtr DebugInputContext_NativeProperty = FPropertyExporter.CallGetNativePropertyFromName(NativeClassPtr, "DebugInputContext");
        DebugInputContext_Offset = FPropertyExporter.CallGetPropertyOffset(DebugInputContext_NativeProperty);
    }
    
    static int InputSubsystem_Offset;
    
    private UnrealSharp.EnhancedInput.UEnhancedInputLocalPlayerSubsystem InputSubsystem
    {
        get
        {
            return ObjectMarshaller<UnrealSharp.EnhancedInput.UEnhancedInputLocalPlayerSubsystem>.FromNative(IntPtr.Add(NativeObject, InputSubsystem_Offset), 0);
        }
    }
    
    
    static int CommonInputContext_Offset;
    
    public UnrealSharp.EnhancedInput.UInputMappingContext CommonInputContext
    {
        get
        {
            return ObjectMarshaller<UnrealSharp.EnhancedInput.UInputMappingContext>.FromNative(IntPtr.Add(NativeObject, CommonInputContext_Offset), 0);
        }
    }
    
    
    static int DragonGroundInputContext_Offset;
    
    public UnrealSharp.EnhancedInput.UInputMappingContext DragonGroundInputContext
    {
        get
        {
            return ObjectMarshaller<UnrealSharp.EnhancedInput.UInputMappingContext>.FromNative(IntPtr.Add(NativeObject, DragonGroundInputContext_Offset), 0);
        }
    }
    
    
    static int DragonFlyingInputContext_Offset;
    
    public UnrealSharp.EnhancedInput.UInputMappingContext DragonFlyingInputContext
    {
        get
        {
            return ObjectMarshaller<UnrealSharp.EnhancedInput.UInputMappingContext>.FromNative(IntPtr.Add(NativeObject, DragonFlyingInputContext_Offset), 0);
        }
    }
    
    
    static int DebugInputContext_Offset;
    
    public UnrealSharp.EnhancedInput.UInputMappingContext DebugInputContext
    {
        get
        {
            return ObjectMarshaller<UnrealSharp.EnhancedInput.UInputMappingContext>.FromNative(IntPtr.Add(NativeObject, DebugInputContext_Offset), 0);
        }
    }
    
    
}