// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "SGameState.generated.h"

UENUM(BlueprintType)
enum class EWaveState : uint8
{
	WaitingToStart,
	WaveInProgress,
	WaitingToComplete,
	WaveComplete,
	GameOver,
};

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
	void OnReq_WaveState(EWaveState OldState);

	UFUNCTION(BlueprintImplementableEvent, Category="GameState")
	void WaveStateChanged(EWaveState NewState, EWaveState OldState);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnReq_WaveState, Category = "GameState")
	EWaveState WaveState;

	UPROPERTY(Replicated)
	int32 Team1KillScore;

	UPROPERTY(Replicated)
	int32 Team2KillScore;
	
	UPROPERTY(EditAnywhere, Category="GameState")
	int32 MaxKillScore;

public:
	void SetWaveState(EWaveState NewState);

	UFUNCTION()
	int GetTeam1KillScore();

	UFUNCTION()
	int GetTeam2KillScore();

	UFUNCTION()
	int GetMaxKillScore();

	UFUNCTION()
	void SetTeam1KillScore(int Score);

	UFUNCTION()
	void SetTeam2KillScore(int Score);


};
