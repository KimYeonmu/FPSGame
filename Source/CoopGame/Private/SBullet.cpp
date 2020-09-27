// Fill out your copyright notice in the Description page of Project Settings.


#include "SBullet.h"

#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "SHealthComponent.h"
#include "Kismet/GameplayStatics.h"
#include "SWeapon.h"

// Sets default values
ASBullet::ASBullet()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ProjectileMovementComp = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComp"));
	ProjectileMovementComp->InitialSpeed = 2000;
	ProjectileMovementComp->MaxSpeed = 2000;
	
	HealthComp = CreateDefaultSubobject<USHealthComponent>(TEXT("HealthComp"));
	HealthComp->SetIsReplicated(true);

	CapsuleComp = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleComp"));
	SetRootComponent(CapsuleComp);
	
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	MeshComp->SetupAttachment(CapsuleComp);

	SetReplicates(true);
}



// Called when the game starts or when spawned
void ASBullet::BeginPlay()
{
	Super::BeginPlay();

	HealthComp->OnHealthChanged.AddDynamic(this, &ASBullet::OnBulletHealthChanged);
}

// Called every frame
void ASBullet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ASBullet::SetTeamNumber(int TeamNum)
{
	HealthComp->TeamNum = TeamNum;
}


void ASBullet::OnBulletHealthChanged(USHealthComponent* OwningHealthComp, float Health, float HealthDelta, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser)
{
	if (Health <= 0.0f)
	{
		Destroy();
	}
}

