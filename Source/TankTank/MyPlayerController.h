// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MyPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class TANKTANK_API AMyPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	
	AMyPlayerController();
	
	UPROPERTY(EditAnywhere,Category=FireTrace)
	bool bFireTraceDebug = false;
	UPROPERTY(BlueprintReadOnly,Category=FireTrace)
	FHitResult FireTraceHit;
protected:
	UPROPERTY()
	AActor* TracingActor;
	UPROPERTY()
	AActor* LastTracedActor;
public:

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;

	void FireTrace();
	void SetOutLine(AActor* TActor,bool bShowOutLine);

};
