// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LoginUserWidget.generated.h"

class UButton;
/**
 * 
 */
UCLASS()
class COOPGAME_API ULoginUserWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeConstruct() override;
	
	UFUNCTION()
	void OnClickCreateButton();

	UFUNCTION()
	void OnClickPlayButton();
	
	UFUNCTION()
	void OnCompleteFindSession();

	UPROPERTY(EditAnywhere, Category = "Button")
	UButton* CreateButton;

	UPROPERTY(EditAnywhere, Category="Button")
	UButton* JoinButton;
	
	UPROPERTY(EditAnywhere, Category = "Level")
	FName NextLevelName;

};
