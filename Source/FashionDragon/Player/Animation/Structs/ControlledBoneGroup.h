#pragma once
#include "FashionDragon/DebugTools/QuickDebug.h"
#include "FashionDragon/Player/Animation/Enums/DriverLayer.h"

template <typename T, typename EffectorT>
using TMemFn = EffectorT (FProceduralPose::*)(const EffectorT&, const T*, const float) const;

template<typename T>
class TFControlledBoneGroup
{
	FName GroupName;
	T* ReferenceBone;
	TArray<T*> ControlledBones;
	T* OutputBone;
public:
	TFControlledBoneGroup(): ReferenceBone(nullptr), OutputBone(nullptr) {};
	TFControlledBoneGroup(FName GroupName, T* ReferenceBone);

	void Tick(const float DeltaTime);

	FName GetGroupName() const { return GroupName; }
	T* GetBone(const EDriverLayer Layer);
	T* GetPostProcessBone();
	TArray<T*> GetAllLayers();
	int GetLayerCount();

	template<typename EffectorT>
	EffectorT MakeEffector(TArray<FProceduralPose*> PoseDrivers, TMemFn<T, EffectorT> EffectorFunc, const float DeltaTime);
	template<typename EffectorT>
	EffectorT MakePostProcessEffector(EffectorT BaseEffector, TArray<FProceduralPose*> PoseDrivers, TMemFn<T, EffectorT> EffectorFunc, const float DeltaTime);
	FPoseWingEffector MakeWingEffector(TArray<FProceduralPose*> PoseDrivers, TMemFn<T, FPoseWingEffector> EffectorFunc, const float DeltaTime);
	FPoseWingEffector MakePostProcessWingEffector(FPoseWingEffector BaseEffector, TArray<FProceduralPose*> PoseDrivers, TMemFn<T, FPoseWingEffector> EffectorFunc, const float DeltaTime);

	// Support iterating
	auto begin() { return ControlledBones.CreateIterator(); }
	auto end() { return ControlledBones.CreateIterator() + ControlledBones.Num(); }
	auto begin() const { return ControlledBones.CreateConstIterator(); }
	auto end() const { return ControlledBones.CreateConstIterator() + ControlledBones.Num(); }
};

template <typename T>
TFControlledBoneGroup<T>::TFControlledBoneGroup(const FName GroupName, T* ReferenceBone): GroupName(GroupName), ReferenceBone(ReferenceBone)
{
	ControlledBones = TArray<T*>();
	OutputBone = new T(*ReferenceBone);
	OutputBone->Layer = EDriverLayer::Output;
	OutputBone->GroupName = GroupName;
	OutputBone->CanProduceEvents = true;
}

template <typename T>
void TFControlledBoneGroup<T>::Tick(const float DeltaTime)
{
	for (auto ControlledBone : ControlledBones)
		ControlledBone->Tick(DeltaTime);
	OutputBone->Tick(DeltaTime);
}

template <typename T>
T* TFControlledBoneGroup<T>::GetBone(const EDriverLayer Layer)
{
	const int LayerIndex = static_cast<int>(Layer);
    while (ControlledBones.Num() <= LayerIndex)
    {
    	const auto NewBone = new T(*ReferenceBone);
    	NewBone->Layer = static_cast<EDriverLayer>(ControlledBones.Num());
    	NewBone->GroupName = GroupName;
    	ControlledBones.Add(NewBone);
    }
    return ControlledBones[LayerIndex];
}

template <typename T>
T* TFControlledBoneGroup<T>::GetPostProcessBone()
{
	return OutputBone;
}

template <typename T>
TArray<T*> TFControlledBoneGroup<T>::GetAllLayers()
{
	return ControlledBones;
}

template <typename T>
int TFControlledBoneGroup<T>::GetLayerCount()
{
	return ControlledBones.Num();
}

template <typename T>
template <typename EffectorT>
EffectorT TFControlledBoneGroup<T>::MakeEffector(TArray<FProceduralPose*> PoseDrivers,
                                                 TMemFn<T, EffectorT> EffectorFunc, const float DeltaTime)
{
	auto CumulativeEffector = EffectorT();
	for (int i = 0; i < ControlledBones.Num(); i++)
	{
		T* Bone = GetBone(static_cast<EDriverLayer>(i));
		auto LocalEffector = FPoseEffector(Bone->Position, Bone->Rotation);
		
		for (const auto PoseDriver : PoseDrivers)
		{
			LocalEffector = (PoseDriver->*EffectorFunc)(LocalEffector, Bone, DeltaTime);
		}
		Bone->Position = LocalEffector.Position;
		Bone->Rotation = LocalEffector.Rotation;
		CumulativeEffector.Position += LocalEffector.Position;
		CumulativeEffector.Rotation += LocalEffector.Rotation;
	}
	return CumulativeEffector;
}

template <typename T>
template <typename EffectorT>
EffectorT TFControlledBoneGroup<T>::MakePostProcessEffector(EffectorT BaseEffector, TArray<FProceduralPose*> PoseDrivers,
												 TMemFn<T, EffectorT> EffectorFunc, const float DeltaTime)
{
	auto Effector = EffectorT(BaseEffector);
	T* Bone = GetPostProcessBone();

	Bone->Position = Effector.Position;
	Bone->Rotation = Effector.Rotation;
	for (const auto PoseDriver : PoseDrivers)
	{
		Effector = (PoseDriver->*EffectorFunc)(Effector, Bone, DeltaTime);
	}
	return Effector;
}

template <typename T>
FPoseWingEffector TFControlledBoneGroup<T>::MakeWingEffector(
	TArray<FProceduralPose*> PoseDrivers,
	const TMemFn<T, FPoseWingEffector> EffectorFunc,
	const float DeltaTime)
{
	auto CumulativeEffector = FPoseWingEffector();
	for (int i = 0; i < ControlledBones.Num(); i++)
	{
		const auto Wing = GetBone(static_cast<EDriverLayer>(i));
		auto CumulativeWingEffector = FPoseWingEffector(Wing->Openness, Wing->Flap, 0.0f);
		for (const auto PoseDriver : PoseDrivers)
		{
			CumulativeWingEffector = (PoseDriver->*EffectorFunc)(CumulativeWingEffector, Wing, DeltaTime);
		}

		Wing->Flap = CumulativeWingEffector.FlapAngle;
		Wing->Openness = CumulativeWingEffector.Openness;
		CumulativeEffector.FlapAngle += CumulativeWingEffector.FlapAngle;
		CumulativeEffector.Openness += CumulativeWingEffector.Openness;
	}
	return CumulativeEffector;
}

template <typename T>
FPoseWingEffector TFControlledBoneGroup<T>::MakePostProcessWingEffector(FPoseWingEffector BaseEffector,
	TArray<FProceduralPose*> PoseDrivers, TMemFn<T, FPoseWingEffector> EffectorFunc, const float DeltaTime)
{
	auto Effector = FPoseWingEffector(BaseEffector);
	T* Bone = GetPostProcessBone();

	Bone->Flap = Effector.FlapAngle;
	Bone->Openness = Effector.Openness;
	for (const auto PoseDriver : PoseDrivers)
	{
		Effector = (PoseDriver->*EffectorFunc)(Effector, Bone, DeltaTime);
	}
	return Effector;
}
