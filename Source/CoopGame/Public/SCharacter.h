// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "SCharacter.generated.h"

class ASWeapon;
class AArrowActor;
class UCameraComponent;
class USpringArmComponent;
class USHealthComponent;
class UUserWidget;
class UPostProcessComponent;
class UInGameBottomUserWidget;
class UResultUserWidget;

UCLASS()
class COOPGAME_API ASCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ASCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void MoveForward(float Value);

	void MoveRight(float Value);

	void BeginCrouch();

	void EndCrouch();

	void Fire();

	UFUNCTION()
	void OnHealthChanged(USHealthComponent* OwningHealthComp, float Health, float HealthDelta, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
	UCameraComponent* CameraComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USpringArmComponent* SpringArmComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USHealthComponent* HealthComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UChildActorComponent* ChildArrowComp;
	
protected:
	UFUNCTION()
	void BeginZoom();

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerBeginZoom();

	UFUNCTION()
	void EndZoom();

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerEndZoom();

	void FinishFixedZoomTimer();

	UPROPERTY(EditDefaultsOnly, Category = "Player")
	float ZoomedFOV;

	UPROPERTY(EditDefaultsOnly, Category = "Player")
	float DefaultFOV;

	UPROPERTY(EditDefaultsOnly, Category = "Player", meta=(ClampMin=0.1, ClampMax=100))
	float ZoomInterpSpeed;
	
	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Player")
	bool bWantsToZoom;

	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Player")
	bool bFinishFixedZoomTimer;

	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Player")
	bool bFixedZoom;

	FTimerHandle TimerHandle_FixedZoom;

protected:
	UPROPERTY(ReplicatedUsing=OnReq_Weapon)
	ASWeapon* CurrentWeapon;

	UFUNCTION()
	void OnReq_Weapon();

	UPROPERTY(EditDefaultsOnly, Category="Player")
	TSubclassOf<ASWeapon> StartWeaponClass;

	UPROPERTY(VisibleDefaultsOnly, Category = "Player")
	FName WeaponAttackSocketName;
	
	UPROPERTY(EditDefaultsOnly, Category="Player")
	TSubclassOf<UUserWidget> KillScoreUserWidgetClass;

	UPROPERTY(EditDefaultsOnly, Category="Player")
	TSubclassOf<UUserWidget> IngameBottomUserWidgetClass;
	
	UPROPERTY(EditDefaultsOnly, Category = "Player")
	TSubclassOf<UUserWidget> ResultWidgetClass;

	UPROPERTY(Replicated, BlueprintReadOnly, Category="Player")
	bool bDied;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual FVector GetPawnViewLocation() const;

	UFUNCTION(BlueprintCallable, Category = "Player")
	void StartFire();

	UFUNCTION(BlueprintCallable, Category = "Player")
	void StopFire();

	uint8 GetTeamNumber();

	UPROPERTY(EditAnywhere, Category="Player")
	bool bAi;

public:
	void SetTimerOutsideMagneticField(float Damage);

	void ClearTimerOutsideMagneticField();

protected:
	FTimerHandle TimerHandle_InsideMagneticField;

	UFUNCTION()
	void OnOutsideMagnaticField();

	float MagnaticFieldDamage;

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Player")
	UPostProcessComponent* PostProcessComp;

protected:
	UPROPERTY()
	UInGameBottomUserWidget* IngameBottomUserWidget;

	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Player")
	int SaveBulletCount;

protected:
	UPROPERTY(Replicated, BlueprintReadOnly, Category="Player")
	bool bReload;

	void Reload();

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerReload();

	UFUNCTION()
	void FinishReload(int RemoveBulletCount);

protected:
	UPROPERTY()
	UResultUserWidget* ResultUserWidget;

protected:
	UFUNCTION()
	void FinishGame();
};
