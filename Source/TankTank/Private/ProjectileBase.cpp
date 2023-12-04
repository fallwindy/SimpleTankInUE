// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectileBase.h"

#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"


AProjectileBase::AProjectileBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bReplicates = true;

	OutCollision = CreateDefaultSubobject<USphereComponent>(TEXT("OutCollision"));
	RootComponent = OutCollision;
	OutCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	OutCollision->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
	OutCollision->SetGenerateOverlapEvents(true);
	OutCollision->OnComponentBeginOverlap.AddDynamic(this,&AProjectileBase::OnOverLap);
	
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(RootComponent);
	Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	
	ProjectileMovementComp = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComp"));
	ProjectileMovementComp->InitialSpeed = 3000.f;
	ProjectileMovementComp->Velocity = this->GetActorForwardVector();
	ProjectileMovementComp->bRotationFollowsVelocity = true;
	ProjectileMovementComp->ProjectileGravityScale = 0.f;

	this->InitialLifeSpan = 7.f;
	

}

// Called when the game starts or when spawned
void AProjectileBase::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AProjectileBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if(UKismetMathLibrary::Vector_Distance(SpawnLocation,GetActorLocation())>FlyRange)
	{
		ProjectileMovementComp->ProjectileGravityScale = 1.f;
	}

}

void AProjectileBase::OnOverLap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(),ExplosionEffect,OverlappedComponent->GetComponentLocation(),FRotator::ZeroRotator,FVector(1),true);
	if (Cast<APawn>(OtherActor))
	{
		UGameplayStatics::ApplyDamage(OtherActor,BaseDamage,UGameplayStatics::GetPlayerController(GetWorld(),0),this,DamageClass);
	}
	Destroy();
	return;
}

