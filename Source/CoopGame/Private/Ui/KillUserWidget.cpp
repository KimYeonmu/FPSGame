// Fill out your copyright notice in the Description page of Project Settings.


#include "Ui/KillUserWidget.h"
#include "SGameState.h"
#include "Components/TextBlock.h"
#include "Components/ProgressBar.h"

void UKillUserWidget::NativeConstruct()
{
	Super::NativeConstruct();

	GS = GetWorld()->GetGameState<ASGameState>();

	Team1KillTextBlock = Cast<UTextBlock>(GetWidgetFromName(TEXT("Team1Text")));
	Team2KillTextBlock = Cast<UTextBlock>(GetWidgetFromName(TEXT("Team2Text")));

	Team1ProgressBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("Team1Bar")));
	Team2ProgressBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("Team2Bar")));
}

void UKillUserWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	
	int32 Team1Score = GS->GetTeam1KillScore();
	int32 Team2Score = GS->GetTeam2KillScore();
	float MaxScore = (float)GS->GetMaxKillScore();

	FString Team1String = FString::FromInt(Team1Score);
	FString Team2String = FString::FromInt(Team2Score);

	Team1KillTextBlock->SetText(FText::FromString(Team1String));
	Team2KillTextBlock->SetText(FText::FromString(Team2String));

	float Team1Percent = FMath::Clamp<float>(Team1Score / MaxScore, 0.01f, 1);
	float Team2Percent = FMath::Clamp<float>(Team2Score / MaxScore, 0.01f, 1);

	Team1ProgressBar->SetPercent(Team1Percent);
	Team2ProgressBar->SetPercent(Team2Percent);
}
