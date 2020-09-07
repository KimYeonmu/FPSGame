// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SHealthComponent.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_SixParams(FOnHealthChangedSignature, USHealthComponent*, HealthComp, float, Health, float, HealthDelta, const class UDamageType*, DamageType, class AController*, InstigatedBy, AActor*, DamageCauser);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class COOPGAME_API USHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	USHealthComponent();

	float GetHealth() const;

	UFUNCTION(BlueprintCallable, Category = "HealthComponent")
	void Heal(float HealAmound);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "HealthComponent")
	static bool IsFriendly(AActor* ActorA, AActor* ActorB);

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="HealthComponent")
	uint8 TeamNum;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UFUNCTION()
	void HandleTakeAnyDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);

	UFUNCTION()
	void OnReq_Health(float OldHelath);
	

protected:
	UPROPERTY(ReplicatedUsing=OnReq_Health, BlueprintReadOnly, Category = "HealthComponent")
	float Health;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HealthComponent")
	float DefaultHealth;

public:
	UPROPERTY(BlueprintAssignable, Category="Events")
	FOnHealthChangedSignature OnHealthChanged;

	UPROPERTY()
	bool bIsDead;
};
