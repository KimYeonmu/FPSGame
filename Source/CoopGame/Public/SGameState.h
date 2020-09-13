// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "SGameState.generated.h"

UENUM(BlueprintType)
enum class EInGameState : uint8
{
	Start,
	End
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FChangeGameState);

/**
 * 
 */
UCLASS()
class COOPGAME_API ASGameState : public AGameStateBase
{
	GENERATED_BODY()
	
	ASGameState();

protected:
	UFUNCTION()
	void OnReq_WaveState(EInGameState OldState);

	UFUNCTION(BlueprintImplementableEvent, Category="GameState")
	void WaveStateChanged(EInGameState NewState, EInGameState OldState);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnReq_WaveState, Category = "GameState")
	EInGameState WaveState;

	UPROPERTY(Replicated)
	int32 Team1KillScore;

	UPROPERTY(Replicated)
	int32 Team2KillScore;
	
	UPROPERTY(EditAnywhere, Category="GameState")
	int32 MaxKillScore;

	
public:
	void SetWaveState(EInGameState NewState);

	UFUNCTION()
	int GetTeam1KillScore();

	UFUNCTION()
	int GetTeam2KillScore();

	UFUNCTION()
	int GetMaxKillScore();

	UFUNCTION()
	int GetWinTeamNumber();
	
	UFUNCTION()
	void SetTeam1KillScore(int Score);

	UFUNCTION()
	void SetTeam2KillScore(int Score);

	UFUNCTION()
	bool IsFinishGame();

	FChangeGameState OnChangeGameState;
};
