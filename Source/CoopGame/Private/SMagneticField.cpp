// Fill out your copyright notice in the Description page of Project Settings.


#include "SMagneticField.h"

#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "SCharacter.h"

// Sets default values
ASMagneticField::ASMagneticField()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	SphereComp->SetupAttachment(RootComponent);

	StaticMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComp"));
	StaticMeshComp->SetupAttachment(SphereComp);

	MagneticFieldDamage = 30;
}

// Called when the game starts or when spawned
void ASMagneticField::BeginPlay()
{
	Super::BeginPlay();
	
	SphereComp->OnComponentBeginOverlap.AddDynamic(this, &ASMagneticField::OnSphereCompBeginOverlap);
	SphereComp->OnComponentEndOverlap.AddDynamic(this, &ASMagneticField::OnSphereCompoEndOverlap);
}

void ASMagneticField::OnSphereCompBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ASCharacter* Character = Cast<ASCharacter>(OtherActor);

	if (Character != nullptr)
	{
		Character->ClearTimerOutsideMagneticField();
	}
}

void ASMagneticField::OnSphereCompoEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	ASCharacter* Character = Cast<ASCharacter>(OtherActor);

	if (Character != nullptr)
	{
		Character->SetTimerOutsideMagneticField(MagneticFieldDamage);
	}
}