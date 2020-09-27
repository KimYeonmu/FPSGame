// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "KillUserWidget.generated.h"

/**
 * 
 */

class UTextBlock;
class UProgressBar;
class ASGameState;

UCLASS()
class COOPGAME_API UKillUserWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeConstruct() override;

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	UPROPERTY(EditAnywhere, Category="Widget")
	UTextBlock* MaxKillTextBlock;

	UPROPERTY(EditAnywhere, Category="Widget")
	UTextBlock* Team1KillTextBlock;

	UPROPERTY(EditAnywhere, Category = "Widget")
	UTextBlock* Team2KillTextBlock;

	UPROPERTY(EditAnywhere, Category="Widget")
	UProgressBar* Team1ProgressBar;

	UPROPERTY(EditAnywhere, Category="Widget")
	UProgressBar* Team2ProgressBar;

private:
	ASGameState* GS;
};
