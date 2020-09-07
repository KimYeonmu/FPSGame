// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SMagneticField.generated.h"

class USphereComponent;
class UStaticMeshComponent;

UCLASS()
class COOPGAME_API ASMagneticField : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASMagneticField();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, Category="MagneticField")
	USphereComponent* SphereComp;

	UPROPERTY(EditAnywhere, Category = "MagneticField")
	UStaticMeshComponent* StaticMeshComp;

	UPROPERTY(EditAnywhere, Category = "MagneticField")
	float MagneticFieldDamage;

protected:
	UFUNCTION()
	void OnSphereCompBeginOverlap(UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor, UPrimitiveComponent* OtherComp, 
		int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnSphereCompoEndOverlap(UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor, UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex);
};
