// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "SGameMode.generated.h"

enum class EWaveState : uint8;


DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnActorKilled, AActor*, VictimActor, AActor*, KillerActor, AController*, KillerController);
/**
 * 
 */

class ASCharacter;

UCLASS()
class COOPGAME_API ASGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
	ASGameMode();

protected:

	UFUNCTION()
	void SpawnNewBot(const FString& TeamName);

	UFUNCTION()
	void SpawnPlayer(ASCharacter* Character);

	void StartWave();

	void EndWave();

	void PrepareForNextWave();

	void SpawnBotTimerElapsed();

	void CheckWaveState();

	void CheckAnyPlayerAlive();

	void GameOver();

	void SetWaveState(EWaveState NewState);

	void RestartDeadPlayers();

	UFUNCTION()
	void OnActorKill(AActor* VictimActor, AActor* KillerActor, AController* KillerController);

public:
	virtual void StartPlay() override;

	virtual void Tick(float DeltaSeconds);

	

protected:
	UPROPERTY(EditDefaultsOnly, Category="GameMode")
	TSubclassOf<ASCharacter> BotCharacter;

	FTimerHandle TimerHandle_BotSpawner;

	FTimerHandle TimerHandle_NextWaveStart;

	int32 NrOfBotsToSpawn;

	int32 WaveCount;

	UPROPERTY(EditDefaultsOnly, Category="GameMode")
	float TimeBetweenWaves;

protected:
	TArray<AActor*> PlayerStartArray;

	TArray<AActor*> GetPlayerStartArray(const FString& TeamName);

	AActor* GetRandomPlayerStart(const FString& TeamName);

	FString GetTeamName(uint8 TeamNumber);

public:
	FOnActorKilled OnActorKilled;
};
