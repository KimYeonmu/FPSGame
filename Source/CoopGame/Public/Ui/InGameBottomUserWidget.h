// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InGameBottomUserWidget.generated.h"

/**
 * 
 */

class UProgressBar;
class UTextBlock;

UCLASS()
class COOPGAME_API UInGameBottomUserWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:

	

	virtual void NativeConstruct() override;

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

public:
	void SetHp(float Hp);

	void SetMaxHp(float MaxHpValue);

protected:
	UPROPERTY(EditAnywhere, Category = "Widget")
	UProgressBar* HpBar;

	UPROPERTY(EditAnywhere, Category = "Widget")
	UProgressBar* HpBarBackground;

	float LerpTime;

	float CurrentHp;

	float PrevHp;

	float MaxHp;

	bool bLerpHp;

	FTimerHandle TimerHandle_LerpHp;

	void SetAnimationLerp();

public:
	void SetBulletText(int BulletCount);

	void SetSaveBulletText(int SaveBulletCount);

protected:
	UTextBlock* BulletText;

	UTextBlock* SaveBulletText;
};
