using UnrealSharp.Attributes;
using UnrealSharp.Core.Attributes;
using UnrealSharp.Interop;
using UnrealSharp.Core.Marshallers;

namespace UnrealSharp.FashionDragon;

[UClass, GeneratedType("BasePlayerController", "UnrealSharp.FashionDragon.BasePlayerController")]
public partial class ABasePlayerController : UnrealSharp.Engine.APlayerController
{
    static readonly IntPtr NativeClassPtr;
    static ABasePlayerController()
    {
        NativeClassPtr = UCoreUObjectExporter.CallGetNativeClassFromName(typeof(ABasePlayerController).GetAssemblyName(), "UnrealSharp.FashionDragon", "BasePlayerController");
    }
    // OnShowAnimDebug
    IntPtr OnShowAnimDebug_NativeFunction;
    
    [UFunction(FunctionFlags.BlueprintEvent), GeneratedType("OnShowAnimDebug")]
    protected virtual void OnShowAnimDebug()
    {
        unsafe
        {
            if (OnShowAnimDebug_NativeFunction == IntPtr.Zero)
            {
                OnShowAnimDebug_NativeFunction = UClassExporter.CallGetNativeFunctionFromInstanceAndName(NativeObject, "OnShowAnimDebug");
            }
            UObjectExporter.CallInvokeNativeFunction(NativeObject, OnShowAnimDebug_NativeFunction, IntPtr.Zero);
        }
    }
    
    // Hide implementation function from Intellisense
    [System.ComponentModel.EditorBrowsable(System.ComponentModel.EditorBrowsableState.Never)]
    protected virtual void OnShowAnimDebug_Implementation()
    {
    }
    void Invoke_OnShowAnimDebug(IntPtr buffer, IntPtr returnBuffer)
    {
        unsafe
        {
            OnShowAnimDebug_Implementation();
        }
    }
    
    // OnDebugButton2
    IntPtr OnDebugButton2_NativeFunction;
    
    [UFunction(FunctionFlags.BlueprintEvent), GeneratedType("OnDebugButton2")]
    protected virtual void OnDebugButton2()
    {
        unsafe
        {
            if (OnDebugButton2_NativeFunction == IntPtr.Zero)
            {
                OnDebugButton2_NativeFunction = UClassExporter.CallGetNativeFunctionFromInstanceAndName(NativeObject, "OnDebugButton2");
            }
            UObjectExporter.CallInvokeNativeFunction(NativeObject, OnDebugButton2_NativeFunction, IntPtr.Zero);
        }
    }
    
    // Hide implementation function from Intellisense
    [System.ComponentModel.EditorBrowsable(System.ComponentModel.EditorBrowsableState.Never)]
    protected virtual void OnDebugButton2_Implementation()
    {
    }
    void Invoke_OnDebugButton2(IntPtr buffer, IntPtr returnBuffer)
    {
        unsafe
        {
            OnDebugButton2_Implementation();
        }
    }
    
    // OnDebugButton3
    IntPtr OnDebugButton3_NativeFunction;
    
    [UFunction(FunctionFlags.BlueprintEvent), GeneratedType("OnDebugButton3")]
    protected virtual void OnDebugButton3()
    {
        unsafe
        {
            if (OnDebugButton3_NativeFunction == IntPtr.Zero)
            {
                OnDebugButton3_NativeFunction = UClassExporter.CallGetNativeFunctionFromInstanceAndName(NativeObject, "OnDebugButton3");
            }
            UObjectExporter.CallInvokeNativeFunction(NativeObject, OnDebugButton3_NativeFunction, IntPtr.Zero);
        }
    }
    
    // Hide implementation function from Intellisense
    [System.ComponentModel.EditorBrowsable(System.ComponentModel.EditorBrowsableState.Never)]
    protected virtual void OnDebugButton3_Implementation()
    {
    }
    void Invoke_OnDebugButton3(IntPtr buffer, IntPtr returnBuffer)
    {
        unsafe
        {
            OnDebugButton3_Implementation();
        }
    }
    
    
}