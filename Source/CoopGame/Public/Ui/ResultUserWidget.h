// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ResultUserWidget.generated.h"

/**
 * 
 */

class UTextBlock;

UCLASS()
class COOPGAME_API UResultUserWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeConstruct() override;

	UPROPERTY(EditAnywhere, Category="Widget")
	UTextBlock* ResultTextBlock;

public:
	void SetResult(bool bWin);
};
