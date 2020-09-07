// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SWeapon.generated.h"

class USkeletalMeshComponent;
class UDamageType;
class UParticleSystem;
class UCameraComponent;
class UChildActorComponent;
class USoundCue;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnFinishReload, int, RemoveBulletCount);

USTRUCT()
struct FHitScanTrace
{
	GENERATED_BODY()

public:

	UPROPERTY()
	TEnumAsByte<EPhysicalSurface> SurfaceType;

	UPROPERTY()
	FVector_NetQuantize TraceTo;


};

UCLASS()
class COOPGAME_API ASWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASWeapon();

	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable, Category = "Weapon")
	virtual void Fire();

	UFUNCTION(Server, Reliable, WithValidation)
	virtual void ServerFire();

	UFUNCTION()
	void StartFire();

	UFUNCTION()
	void StopFire();

	UFUNCTION()
	void OnReq_HitScanTrace();

	UFUNCTION()
	void PlayFireEffect(FVector TraceEnd);

	void PlayImpactEffects(EPhysicalSurface SurfaceType, FVector ImpactPoint);
	
	UFUNCTION()
	AActor* GetChildAimActor();

	UFUNCTION()
	float GetBaseDamage();

public:
	UFUNCTION()
	void Reload(int BulletNum);

	UFUNCTION()
	int GetBulletCount();

	UFUNCTION()
	int GetMaxBulletCount();

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Components")
	USkeletalMeshComponent* MeshComp;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Weapon")
	TSubclassOf<UDamageType> DamageType;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	FName MuzzleSocketName;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	FName TracerTargetName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	UParticleSystem* MuzzleEffect;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	UParticleSystem* FleshImpactEffect;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	UParticleSystem* DefaultImpactEffect;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	UParticleSystem* TracerEffect;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	UChildActorComponent* ChildArrowComp;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	TSubclassOf<UCameraShake> FireCamShake;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	float BaseDamage;

	FTimerHandle TimerHandle_TimeBetweenShots;

	float LastFireTime;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	float RateOfFire;
	
	UPROPERTY(EditDefaultsOnly, Category = "Weapon", meta=(ClampMin=0.0f))
	float BulletSpread;

	float TimeBetweenShots;

	UPROPERTY(ReplicatedUsing=OnReq_HitScanTrace)
	FHitScanTrace HitScanTrace;

protected:
	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Player")
	int BulletCount;

	int MaxBulletCount;

	UPROPERTY(EditAnywhere, Category="Weapon")
	float ReloadDelay;

	FTimerHandle TimerHandle_Reload;

	void OnReload(int BulletNum);

public:
	UPROPERTY()
	FOnFinishReload OnFinishReload;

protected:
	UPROPERTY(EditDefaultsOnly, Category="Weapon")
	USoundCue* FireSound;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	USoundCue* ReloadSound;
};
