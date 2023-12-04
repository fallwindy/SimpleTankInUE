// Fill out your copyright notice in the Description page of Project Settings.


#include "MyPlayerController.h"
#include "MyTankBase.h"
#include "Kismet/KismetSystemLibrary.h"

AMyPlayerController::AMyPlayerController()
{
}

void AMyPlayerController::BeginPlay()
{
	Super::BeginPlay();
}

void AMyPlayerController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	FireTrace();
}

void AMyPlayerController::FireTrace()
{
	if (auto MyTank = Cast<AMyTankBase>(GetPawn()))
	{
		FVector Start = MyTank->Mesh->GetSocketLocation(MyTank->MuzzleSocketName);
		FVector End = Start + MyTank->Mesh->GetSocketRotation(MyTank->MuzzleSocketName).Vector() * MyTank->FireRange;
		
		UKismetSystemLibrary::SphereTraceSingleForObjects(
			GetWorld(),
			Start,
			End,
			5.f,
			{UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_Pawn),UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_WorldStatic)},
			false,
			{MyTank},
			bFireTraceDebug ? EDrawDebugTrace::ForOneFrame : EDrawDebugTrace::None ,
			FireTraceHit,
			true
		);
		
		if (FireTraceHit.GetActor() != TracingActor)
		{
			LastTracedActor = TracingActor;
			TracingActor = FireTraceHit.GetActor();
			SetOutLine(LastTracedActor,false);
			if(Cast<APawn>(TracingActor))SetOutLine(TracingActor,true);
			
		}
	}
	
}

void AMyPlayerController::SetOutLine(AActor* TActor, bool bShowOutLine)
{
	if(!TActor)return;
	TArray<UMeshComponent*> MeshComps;
	TActor->GetComponents(MeshComps);
	for (auto MeshComp : MeshComps)
	{
		if (!bShowOutLine)
		{
			MeshComp->SetRenderCustomDepth(false);
			continue;
		}
		MeshComp->SetRenderCustomDepth(true);
		MeshComp->SetCustomDepthStencilValue(1);
	}
}
