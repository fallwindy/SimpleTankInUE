// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TankMoveComponent.generated.h"

USTRUCT(BlueprintType)
struct FWheelInfo
{
	GENERATED_BODY()
	UPROPERTY()
	USceneComponent* WheelEmptyScene;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	FName SceneCompName;
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	float LastTraceDistance;
	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite)
	float WheelSpeed;
	
};

UCLASS(Blueprintable, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TANKTANK_API UTankMoveComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UTankMoveComponent();

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category=Wheels)
	TArray<FWheelInfo> LeftWheels;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category=Wheels)
	TArray<FWheelInfo> RightWheels;

	UPROPERTY(EditAnywhere,Category=Suspension)
	float SpringForceMulti = 200.f;
	UPROPERTY(EditAnywhere,Category=Suspension)
	float SpringDumping = 2400.f;
	UPROPERTY(EditAnywhere,Category=Suspension)
	float SuspensionDefaultLength = 110.f;
	UPROPERTY(EditAnywhere,Category=Suspension)
	float TraceLength = 130.f;
	UPROPERTY(EditAnywhere,Category=Suspension)
	float TraceRadius = 32.f;
	UPROPERTY(EditAnywhere,Category=Suspension)
	bool bDebug;

	UPROPERTY(EditAnywhere,Category=Propel)
	float PropelForceMulti = 200.f;
	UPROPERTY(EditAnywhere,Category=Propel)
	float MaxPropelSpeed = 1000.f;
	UPROPERTY(EditAnywhere,Category=Propel)
	float DefaultBrakeMulti = 5.f;
	UPROPERTY(EditAnywhere,Category=Propel)
	float LeftTrackThrottle;
	UPROPERTY(EditAnywhere,Category=Propel)
	float RightTrackThrottle;

	UPROPERTY(EditAnywhere,Category=SideFriction)
	float SideFrictionForceMulti = 2000.f;

	UPrimitiveComponent* OwnerRoot;
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	void InitializeWheels();

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void CalcTotalForce(FWheelInfo& WheelInfo,float Throttle);
	FVector CalcSpringForce(const FWheelInfo& WheelInfo,const FHitResult& Hit,const FVector& WheelLocalVelocity);
	FVector CalcPropelForce(const FWheelInfo& WheelInfo,const FHitResult& Hit,const FVector& WheelLocalVelocity,float PropelInput);
	FVector CalcSideFrictionForce(const FWheelInfo& WheelInfo,const FHitResult& Hit,const FVector& WheelLocalVelocity);

	UFUNCTION(BlueprintCallable)
	void SetMovementInput(float LeftInput,float RightInput);
};
