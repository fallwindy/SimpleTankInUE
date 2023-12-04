
#include "TankTank/Public/MyTankBase.h"
#include "BehaviorTree/BehaviorTreeTypes.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystem.h"


AMyTankBase::AMyTankBase()
{
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh"));
	RootComponent = Mesh;
	FireEffect = CreateDefaultSubobject<UParticleSystem>(TEXT("FireEffect"));
	ReckonTime = ReloadTime;
	CurHealth = MaxHealth;

}

// Called when the game starts or when spawned
void AMyTankBase::BeginPlay()
{
	Super::BeginPlay();
	CurHealth = MaxHealth;
}

float AMyTankBase::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator,
	AActor* DamageCauser)
{
	CurHealth -= DamageAmount;
	BPDamage();
	return Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
}


void AMyTankBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if(ReckonTime<ReloadTime)
	{
		ReckonTime += DeltaTime; 
	}

}


void AMyTankBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAxis("LookUp",this,&AMyTankBase::LookUp);
	PlayerInputComponent->BindAxis("LookRight",this,&AMyTankBase::LookRight);

	PlayerInputComponent->BindAction("Fire",IE_Pressed,this,&AMyTankBase::Fire);

}



void AMyTankBase::LookUp(float val)
{
	AddControllerPitchInput(-val);
}

void AMyTankBase::LookRight(float val)
{
	AddControllerYawInput(val);
}




void AMyTankBase::Fire()
{
	if(ReckonTime<ReloadTime)return;
	FTransform FireTransform = Mesh->GetSocketTransform(MuzzleSocketName);
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	auto Shell = GetWorld()->SpawnActor<AProjectileBase>(ShellClass,FireTransform,SpawnParams);
	if (Shell)
	{
		Shell->SpawnLocation = FireTransform.GetLocation();
		Shell->FlyRange = FireRange;
	}
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(),FireEffect,FireTransform,true);
	ReckonTime = 0;
	
}



void AMyTankBase::BPDamage_Implementation()
{
}

