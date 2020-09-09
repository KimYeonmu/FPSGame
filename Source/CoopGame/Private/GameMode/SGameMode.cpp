// Fill out your copyright notice in the Description page of Project Settings.


#include "GameMode/SGameMode.h"
#include "TimerManager.h"
#include "SHealthComponent.h"
#include "SGameState.h"
#include "SPlayerState.h"
#include "SCharacter.h"
#include "GameFramework/PlayerStart.h"
#include "Kismet/GameplayStatics.h"

ASGameMode::ASGameMode()
{
	TimeBetweenWaves = 2;

	GameStateClass = ASGameState::StaticClass();
	PlayerStateClass = ASPlayerState::StaticClass();

	PrimaryActorTick.bCanEverTick = true;
	//PrimaryActorTick.TickInterval = 1.0f;
}

void ASGameMode::SpawnNewBot(const FString& TeamName)
{
	AActor* RandomStartActor = GetRandomPlayerStart(TeamName);

	FVector Position = RandomStartActor->GetActorLocation();
	FRotator Rotator = RandomStartActor->GetActorRotation();

	GetWorld()->SpawnActor(BotCharacter, &Position, &Rotator);
}


void ASGameMode::SpawnPlayer(ASCharacter* Character)
{
	FString TeamName = GetTeamName(Character->GetTeamNumber());
	AActor* RandomStartActor = GetRandomPlayerStart(TeamName);
	
	if (RandomStartActor != nullptr)
	{
		FTimerDelegate RespawnDelegate = FTimerDelegate::CreateUObject(this,
			&ASGameMode::RestartPlayerAtPlayerStart,
			Character->GetController(),
			RandomStartActor);

		FTimerHandle SpawnTimer;
		//RestartPlayerAtPlayerStart(Character->GetController(), RandomStartActor);

		APlayerController* PS = Cast<APlayerController>(Character->GetController());
		//RestartPlayer(PS);
		//RestartDeadPlayers();
		GetWorldTimerManager().SetTimer(SpawnTimer, RespawnDelegate, 5, false);

	}
}

void ASGameMode::StartWave()
{
	SpawnNewBot("Team2");
	SpawnNewBot("Team2");
	SpawnNewBot("Team2");
	SpawnNewBot("Team2");

	// WaveCount++;
	// NrOfBotsToSpawn = 2 * WaveCount;
	// GetWorldTimerManager().SetTimer(TimerHandle_BotSpawner, this, &ASGameMode::SpawnBotTimerElapsed, 1, true, 0.0f);

	SetWaveState(EWaveState::WaveInProgress);
}

void ASGameMode::EndWave()
{
	GetWorldTimerManager().ClearTimer(TimerHandle_BotSpawner);

	SetWaveState(EWaveState::WaitingToComplete);
}

void ASGameMode::PrepareForNextWave()
{
	GetWorldTimerManager().SetTimer(TimerHandle_NextWaveStart, this, &ASGameMode::StartWave, TimeBetweenWaves, false);

	SetWaveState(EWaveState::WaitingToStart);

	RestartDeadPlayers();
}

void ASGameMode::SpawnBotTimerElapsed()
{
	// SpawnNewBot();

	NrOfBotsToSpawn--;
	
	if (NrOfBotsToSpawn <= 0)
	{
		EndWave();
	}
}

void ASGameMode::CheckWaveState()
{
	bool bIsPreparingForWave = GetWorldTimerManager().IsTimerActive(TimerHandle_NextWaveStart);

	if (NrOfBotsToSpawn > 0 || bIsPreparingForWave)
	{
		return;
	}

	bool bIsAnyBotAlive = false;

	for (FConstPawnIterator It = GetWorld()->GetPawnIterator(); It; ++It)
	{
		APawn* TestPawn = It->Get();

		if (TestPawn == nullptr || TestPawn->IsPlayerControlled())
		{
			continue;
		}

		USHealthComponent* HealthComp = Cast<USHealthComponent>(TestPawn->GetComponentByClass(USHealthComponent::StaticClass()));

		if (HealthComp && HealthComp->GetHealth() > 0.0f)
		{
			bIsAnyBotAlive = true; 
			break;
		}
	}
		
	if (!bIsAnyBotAlive)
	{
		SetWaveState(EWaveState::WaveComplete);
		PrepareForNextWave();
	}
}

void ASGameMode::CheckAnyPlayerAlive()
{
	for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator();
		It; ++It)
	{
		APlayerController* PC = It->Get();

		if (PC && PC->GetPawn())
		{
			APawn* MyPawn = PC->GetPawn();

			USHealthComponent* HealthComp = Cast<USHealthComponent>(MyPawn->GetComponentByClass(USHealthComponent::StaticClass()));

			if (ensure(HealthComp) && HealthComp->GetHealth() > 0.0f)
			{
				return;
			}
		}
	}
	
	GameOver();
}

void ASGameMode::GameOver()
{
	EndWave();

	SetWaveState(EWaveState::GameOver);
	UE_LOG(LogTemp, Log, TEXT("Player Die"));
}

void ASGameMode::SetWaveState(EWaveState NewState)
{
	ASGameState* GS = GetGameState<ASGameState>();

	if (ensureAlways(GS))
	{
		GS->SetWaveState(NewState);
	}
}

void ASGameMode::RestartDeadPlayers()
{
	for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator();
		It; ++It)
	{
		APlayerController* PC = It->Get();
		
		if (PC && PC->GetPawn() == nullptr)
		{
			RestartPlayer(PC);
		}
	}
}

void ASGameMode::OnActorKill(AActor* VictimActor, AActor* KillerActor, AController* KillerController)
{
	ASCharacter* Character = Cast<ASCharacter>(KillerActor);
	ASGameState* GS = GetGameState<ASGameState>();


	if (Character != nullptr && GS != nullptr)
	{
		if (Character->GetTeamNumber() == 0)
		{
			int Score = GS->GetTeam1KillScore();
			GS->SetTeam1KillScore(Score + 1);
		}
		else
		{
			int Score = GS->GetTeam2KillScore();
			GS->SetTeam2KillScore(Score + 1);
		}

		if (!Character->bAi)
			SpawnPlayer(Character);
		else
			SpawnNewBot(GetTeamName(Character->GetTeamNumber()));
	}
}

void ASGameMode::StartPlay()
{
	Super::StartPlay();

	PrepareForNextWave();

	OnActorKilled.AddDynamic(this, &ASGameMode::OnActorKill);

	UGameplayStatics::GetAllActorsOfClass(GetWorld(), 
		APlayerStart::StaticClass(),
		PlayerStartArray);
}

void ASGameMode::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	// CheckWaveState();
	// CheckAnyPlayerAlive();
}

TArray<AActor*> ASGameMode::GetPlayerStartArray(const FString& TeamName)
{
	TArray<AActor*> TeamArray;

	for (AActor* PlayerStartActor : PlayerStartArray)
	{
		APlayerStart* PlayerStart = Cast<APlayerStart>(PlayerStartActor);

		if (!PlayerStart->PlayerStartTag.ToString().Compare(TeamName))
		{
			TeamArray.Add(PlayerStartActor);
		}
	}

	return TeamArray;
}

AActor* ASGameMode::GetRandomPlayerStart(const FString& TeamName)
{
	TArray<AActor*> TeamArray = GetPlayerStartArray(TeamName);

	AActor* RandomStartActor = nullptr;

	if (TeamArray.Num() > 0)
	{
		float random = (int)FMath::RandRange(0, TeamArray.Num() - 1);
		RandomStartActor = TeamArray[random];
	}
	
	return RandomStartActor;
}

FString ASGameMode::GetTeamName(uint8 TeamNumber)
{
	if (TeamNumber == 0)
	{
		return FString("Team1");
	}
	
	return FString("Team2");
}

