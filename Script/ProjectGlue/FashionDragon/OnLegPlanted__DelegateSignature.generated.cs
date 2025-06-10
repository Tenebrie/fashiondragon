using UnrealSharp.Attributes;
using UnrealSharp.Core.Attributes;
using UnrealSharp.Interop;
using UnrealSharp.Core.Marshallers;

namespace UnrealSharp.FashionDragon;


[GeneratedType("OnLegPlanted__DelegateSignature"), UMultiDelegate]
public delegate void OnLegPlanted(UnrealSharp.CoreUObject.FVector spawnLocation);

public partial class UOnLegPlanted : MulticastDelegate<OnLegPlanted>
{
    // FOnLegPlanted
    static IntPtr FOnLegPlanted_NativeFunction;
    static int FOnLegPlanted_ParamsSize;
    static int FOnLegPlanted_SpawnLocation_Offset;
    
    protected void Invoker(UnrealSharp.CoreUObject.FVector spawnLocation)
    {
        unsafe
        {
            byte* paramsBufferAllocation = stackalloc byte[FOnLegPlanted_ParamsSize];
            nint paramsBuffer = (nint) paramsBufferAllocation;
            BlittableMarshaller<UnrealSharp.CoreUObject.FVector>.ToNative(IntPtr.Add(paramsBuffer, FOnLegPlanted_SpawnLocation_Offset), 0, spawnLocation);
            
            ProcessDelegate(paramsBuffer);
            
        }
    }
    static public void InitializeUnrealDelegate(IntPtr nativeDelegateProperty)
    {
        FOnLegPlanted_NativeFunction = FMulticastDelegatePropertyExporter.CallGetSignatureFunction(nativeDelegateProperty);
        FOnLegPlanted_ParamsSize = UFunctionExporter.CallGetNativeFunctionParamsSize(FOnLegPlanted_NativeFunction);
        FOnLegPlanted_SpawnLocation_Offset = FPropertyExporter.CallGetPropertyOffsetFromName(FOnLegPlanted_NativeFunction, "SpawnLocation");
    }
}