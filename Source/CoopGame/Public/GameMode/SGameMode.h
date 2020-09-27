// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "SGameMode.generated.h"

enum class EInGameState : uint8;


DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnActorKilled, AActor*, VictimActor, AActor*, DefeatActor, AController*, VictimController);
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

	void CheckAnyPlayerAlive();

	void SetWaveState(EInGameState NewState);

	void RestartDeadPlayers();

	UFUNCTION()
	void OnActorKill(AActor* VictimActor, AActor* DefeatActor, AController* KillerController);

public:
	virtual void StartPlay() override;

	virtual void Tick(float DeltaSeconds);

	FString GetTeamName(uint8 TeamNumber);

	AActor* GetRandomPlayerStart(const FString& TeamName);

protected:
	UPROPERTY(EditDefaultsOnly, Category="GameMode")
	TSubclassOf<ASCharacter> BotCharacter;

	FTimerHandle TimerHandle_BotSpawner;

protected:
	TArray<AActor*> PlayerStartArray;

	TArray<AActor*> GetPlayerStartArray(const FString& TeamName);
	

public:
	FOnActorKilled OnActorKilled;
};
