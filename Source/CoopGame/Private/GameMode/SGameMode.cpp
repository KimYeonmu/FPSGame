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
}

void ASGameMode::SetWaveState(EInGameState NewState)
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

void ASGameMode::OnActorKill(AActor* VictimActor, AActor* DefeatActor, AController* VictimController)
{
	ASCharacter* WinCharacter = Cast<ASCharacter>(VictimActor);
	ASCharacter* DefectCharacter = Cast<ASCharacter>(DefeatActor);

	ASGameState* GS = GetGameState<ASGameState>();


	if (WinCharacter != nullptr && GS != nullptr)
	{
		if (WinCharacter->GetTeamNumber() == 0)
		{
			int Score = GS->GetTeam1KillScore();
			GS->SetTeam1KillScore(Score + 1);
		}
		else
		{
			int Score = GS->GetTeam2KillScore();
			GS->SetTeam2KillScore(Score + 1);
		}

		if (GS->IsFinishGame())
		{
			SetWaveState(EInGameState::End);
			return;
		}

		if (!DefectCharacter->bAi)
		{
			SpawnPlayer(DefectCharacter);
		}
		
		else
		{
			SpawnNewBot(GetTeamName(DefectCharacter->GetTeamNumber()));
		}
	}
}

void ASGameMode::StartPlay()
{
	Super::StartPlay();

	OnActorKilled.AddDynamic(this, &ASGameMode::OnActorKill);

	UGameplayStatics::GetAllActorsOfClass(GetWorld(),
		APlayerStart::StaticClass(),
		PlayerStartArray);

	SetWaveState(EInGameState::Start);
	StartWave();
}

void ASGameMode::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
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

