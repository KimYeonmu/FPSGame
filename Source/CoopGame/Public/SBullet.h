// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SBullet.generated.h"

class UProjectileMovementComponent;
class USHealthComponent;
class UCapsuleComponent;
class ASWeapon;


UCLASS()
class COOPGAME_API ASBullet : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASBullet();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Bullet")
	UCapsuleComponent* CapsuleComp;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, Category="Bullet")
	UProjectileMovementComponent* ProjectileMovementComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Bullet")
	USHealthComponent* HealthComp;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Bullet")
	TSubclassOf<UDamageType> BulletDamageType;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Bullet")
	UStaticMeshComponent* MeshComp;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void SetTeamNumber(int TeamNum);

	UFUNCTION()
	void OnBulletHealthChanged(USHealthComponent* OwningHealthComp, float Health, float HealthDelta, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);
};
