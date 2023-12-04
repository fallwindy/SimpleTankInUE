// Fill out your copyright notice in the Description page of Project Settings.


#include "TankTank/Public/TankMoveComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"

// Sets default values for this component's properties
UTankMoveComponent::UTankMoveComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UTankMoveComponent::BeginPlay()
{
	Super::BeginPlay();

	OwnerRoot = Cast<UPrimitiveComponent>(GetOwner()->GetRootComponent());
	if(!OwnerRoot)Deactivate();
	
	InitializeWheels();
	
}

void UTankMoveComponent::InitializeWheels()
{
	TArray<USceneComponent*> AllSceneComps;
	GetOwner()->GetComponents(AllSceneComps);
	for (auto &Wheel : LeftWheels)
	{
		for (auto &SceneComp : AllSceneComps)
		{
			if (SceneComp->GetFName()==Wheel.SceneCompName)
			{
				Wheel.WheelEmptyScene = SceneComp;
				break;
			}
		}
	}
	for (auto &Wheel : RightWheels)
	{
		for (auto &SceneComp : AllSceneComps)
		{
			if (SceneComp->GetFName()==Wheel.SceneCompName)
			{
				Wheel.WheelEmptyScene = SceneComp;
				break;
			}
		}
	}
}


// Called every frame
void UTankMoveComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	for (auto &Wheel : LeftWheels)
	{
		CalcTotalForce(Wheel,LeftTrackThrottle);
	}
	for (auto &Wheel : RightWheels)
	{
		CalcTotalForce(Wheel,RightTrackThrottle);
	}
}


void UTankMoveComponent::CalcTotalForce(FWheelInfo& WheelInfo,float Throttle)
{
	FVector Start = WheelInfo.WheelEmptyScene->GetComponentLocation();
	FVector End = Start - (WheelInfo.WheelEmptyScene->GetUpVector() * TraceLength);
	FHitResult Hit;
	if(!UKismetSystemLibrary::SphereTraceSingleForObjects(
		GetWorld(),
		Start,
		End,
		TraceRadius,
		{UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_WorldDynamic),UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_WorldStatic)},
		false,
		{GetOwner()},
		bDebug ? EDrawDebugTrace::ForOneFrame : EDrawDebugTrace::None,
		Hit,
		true
		)) {return;}

	WheelInfo.LastTraceDistance = Hit.Distance;
	
	auto WheelLocalVelocity = OwnerRoot->GetPhysicsLinearVelocityAtPoint(WheelInfo.WheelEmptyScene->GetComponentLocation());
	WheelLocalVelocity = UKismetMathLibrary::InverseTransformDirection(WheelInfo.WheelEmptyScene->GetComponentTransform(),WheelLocalVelocity);

	WheelInfo.WheelSpeed = WheelLocalVelocity.X;
	
	FVector TotalForce;
	TotalForce += CalcSpringForce(WheelInfo,Hit,WheelLocalVelocity);
	TotalForce += CalcPropelForce(WheelInfo,Hit,WheelLocalVelocity,Throttle);
	TotalForce += CalcSideFrictionForce(WheelInfo,Hit,WheelLocalVelocity);
	
	OwnerRoot->AddForceAtLocation(TotalForce,WheelInfo.WheelEmptyScene->GetComponentLocation());
}

//计算弹力
FVector UTankMoveComponent::CalcSpringForce(const FWheelInfo& WheelInfo,const FHitResult& Hit,const FVector& WheelLocalVelocity)
{
	return Hit.Normal * ((SuspensionDefaultLength - Hit.Distance) * SpringForceMulti - WheelLocalVelocity.Z * SpringDumping);
}

//计算推进力
FVector UTankMoveComponent::CalcPropelForce(const FWheelInfo& WheelInfo, const FHitResult& Hit,
	const FVector& WheelLocalVelocity, float PropelInput)
{
	FVector ResultForce;
	auto TargetSpeed = MaxPropelSpeed * PropelInput;
	ResultForce = WheelInfo.WheelEmptyScene->GetForwardVector() * FMath::Clamp(TargetSpeed - WheelLocalVelocity.X,-800.f,800.f) * PropelForceMulti * (1.f + Hit.Normal.Z);
	if(TargetSpeed==0.f) ResultForce *= DefaultBrakeMulti;
	return ResultForce;
}

//计算侧滑摩擦力
FVector UTankMoveComponent::CalcSideFrictionForce(const FWheelInfo& WheelInfo, const FHitResult& Hit,
	const FVector& WheelLocalVelocity)
{
	return WheelInfo.WheelEmptyScene->GetRightVector() * WheelLocalVelocity.Y * (-1.f * SideFrictionForceMulti);
}

void UTankMoveComponent::SetMovementInput(float LeftInput, float RightInput)
{
	LeftTrackThrottle = LeftInput;
	RightTrackThrottle = RightInput;
}

