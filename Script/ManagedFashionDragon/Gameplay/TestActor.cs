using UnrealSharp.Attributes;
using UnrealSharp.Engine;
using UnrealSharp.FashionDragon;

namespace ManagedFashionDragon.Gameplay;

[UClass]
public class AMyTestClass13 : AActor
{   
    [UProperty(PropertyFlags.BlueprintReadOnly)]
    public int MyInt { get; set; }

    protected override void BeginPlay()
    {
        PrintString("Hello from C# 2!");
        AMainCharacter mainCharacter;
        base.BeginPlay();
    }

    [UFunction(FunctionFlags.BlueprintCallable)]
    public void MyFunction(bool myBool, int MyInt)
    {
        PrintString("Hello from MyFunction!");
    }
}