// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SExplosiveBarrel.generated.h"

class USHealthComponent;
class UStaticMeshComponent;
class URadialForceComponent;
class UParticleSystem;

UCLASS()
class COOPGAME_API ASExplosiveBarrel : public AActor
{
	GENERATED_BODY()
	
protected:
	// Sets default values for this actor's properties
	ASExplosiveBarrel();

	UPROPERTY(VisibleAnywhere, Category = "Components")
	USHealthComponent* HealthComp;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	UStaticMeshComponent* MeshComp;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	URadialForceComponent* RadialForceComp;

	UPROPERTY(EditDefaultsOnly, Category = "FX")
	UParticleSystem* ExplosionEffect;

	UPROPERTY(EditDefaultsOnly, Category = "FX")
	UMaterialInterface* ExplodedMaterial;

	UPROPERTY(ReplicatedUsing = OnReq_Exploded)
	bool bExploded;

	UPROPERTY(EditDefaultsOnly, Category = "FX")
	float ExplosionImpulse;

public:

	UFUNCTION()
	void OnHealthChanged(USHealthComponent* OwningHealthComp, float Health, float HealthDelta, const class UDamageType* DamageType,
			class AController* InstigatedBy, AActor* DamageCauser);

	UFUNCTION()
	void OnReq_Exploded();


};
