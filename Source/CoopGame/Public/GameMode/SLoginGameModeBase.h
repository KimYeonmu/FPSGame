// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "SLoginGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class COOPGAME_API ASLoginGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
	
	ASLoginGameModeBase();

protected:
	virtual void BeginPlay() override;

	virtual TSubclassOf<AGameSession> GetGameSessionClass() const override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Login")
	TSubclassOf<UUserWidget> LoginWidgetClass;


};
