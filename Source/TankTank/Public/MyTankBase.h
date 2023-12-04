// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Crosshair.h"
#include "ProjectileBase.h"
#include "GameFramework/Pawn.h"
#include "MyTankBase.generated.h"

UCLASS()
class TANKTANK_API AMyTankBase : public APawn
{
	GENERATED_BODY()

public:
	
	AMyTankBase();

	UPROPERTY(VisibleAnywhere,BlueprintReadWrite)
	USkeletalMeshComponent* Mesh;

	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite,Category=Fire)
	TSubclassOf<AProjectileBase> ShellClass;
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category=Fire)
    float FireRange = 10000.f;
	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite,Category=Fire)
	float ReloadTime = 3.f;
	UPROPERTY(EditDefaultsOnly,Category=Fire)
	FName MuzzleSocketName;
	UPROPERTY(EditDefaultsOnly,Category=Fire)
	UParticleSystem*  FireEffect;
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category=Fire)
	float ReckonTime;

	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite)
	float MaxHealth = 30;
	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite)
	float CurHealth;
	


protected:
	
	virtual void BeginPlay() override;
	
	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
	
	

public:	
	
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	void LookUp(float val);
	void LookRight(float val);
	
	
	UFUNCTION(BlueprintCallable)
	void Fire();

	UFUNCTION(BlueprintNativeEvent)
	void BPDamage();



};
