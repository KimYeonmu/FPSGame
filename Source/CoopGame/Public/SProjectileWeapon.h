// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SWeapon.h"
#include "SProjectileWeapon.generated.h"

/**
 * 
 */
UCLASS()
class COOPGAME_API ASProjectileWeapon : public ASWeapon
{
	GENERATED_BODY()
	
protected:
	virtual void Fire() override;

	virtual void ServerFire_Implementation() override;

	virtual bool ServerFire_Validate() override;

	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComponent,
		AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse,
		const FHitResult& Hit);

protected:
	UPROPERTY(EditDefaultsOnly, Category="ProjectileWeapon")
	TSubclassOf<AActor> ProjectileClass;
};
