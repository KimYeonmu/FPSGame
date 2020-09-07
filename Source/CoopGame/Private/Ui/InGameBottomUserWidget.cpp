// Fill out your copyright notice in the Description page of Project Settings.


#include "Ui/InGameBottomUserWidget.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"

void UInGameBottomUserWidget::NativeConstruct()
{
	Super::NativeConstruct();

	HpBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("HP_Bar")));
	HpBarBackground = Cast<UProgressBar>(GetWidgetFromName(TEXT("HPBar_Background")));

	BulletText = Cast<UTextBlock>(GetWidgetFromName(TEXT("BulletText")));
	SaveBulletText = Cast<UTextBlock>(GetWidgetFromName(TEXT("SaveBulletText")));

	LerpTime = 1;

}

void UInGameBottomUserWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	if (bLerpHp && PrevHp != CurrentHp)
	{
		PrevHp = FMath::Lerp<float, float>(PrevHp, CurrentHp, LerpTime);
		HpBarBackground->SetPercent(PrevHp / MaxHp);
		LerpTime += InDeltaTime;
	}
}

void UInGameBottomUserWidget::SetAnimationLerp()
{
	bLerpHp = true;
}

void UInGameBottomUserWidget::SetBulletText(int BulletCount)
{
	BulletText->SetText(FText::FromString(FString::FromInt(BulletCount)));
}

void UInGameBottomUserWidget::SetSaveBulletText(int SaveBulletCount)
{
	SaveBulletText->SetText(FText::FromString(FString::FromInt(SaveBulletCount)));
}

void UInGameBottomUserWidget::SetHp(float Hp)
{
	bLerpHp = false;

	LerpTime = 0;
	PrevHp = CurrentHp;
	CurrentHp = FMath::Clamp<float>(Hp, 0, MaxHp);

	HpBarBackground->SetPercent(PrevHp / MaxHp);
	HpBar->SetPercent(CurrentHp / MaxHp);
	
	GetWorld()->GetTimerManager().SetTimer(TimerHandle_LerpHp, this, 
		&UInGameBottomUserWidget::SetAnimationLerp,
		0.5f);
}

void UInGameBottomUserWidget::SetMaxHp(float MaxHpValue)
{
	MaxHp = MaxHpValue;
	CurrentHp = MaxHpValue;
	PrevHp = MaxHpValue;
}
