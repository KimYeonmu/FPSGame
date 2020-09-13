// Fill out your copyright notice in the Description page of Project Settings.


#include "SGameState.h"
#include "Net/UnrealNetwork.h"

ASGameState::ASGameState()
{
	MaxKillScore = 5;
}

void ASGameState::OnReq_WaveState(EInGameState OldState)
{
	WaveStateChanged(WaveState, OldState);
	OnChangeGameState.Broadcast();
}

void ASGameState::SetWaveState(EInGameState NewState)
{
	if (HasAuthority())
	{
		EInGameState OldState = WaveState;

		WaveState = NewState;

		OnReq_WaveState(OldState);
	}
}

int ASGameState::GetTeam1KillScore()
{
	return Team1KillScore;
}

int ASGameState::GetTeam2KillScore()
{
	return Team2KillScore;
}

int ASGameState::GetMaxKillScore()
{
	return MaxKillScore;
}

int ASGameState::GetWinTeamNumber()
{
	if (Team1KillScore >= MaxKillScore)
		return 0;

	return 255;
}

void ASGameState::SetTeam1KillScore(int Score)
{
	Team1KillScore = Score;
}

void ASGameState::SetTeam2KillScore(int Score)
{
	Team2KillScore = Score;
}

bool ASGameState::IsFinishGame()
{
	return Team1KillScore >= MaxKillScore ||
		Team2KillScore >= MaxKillScore;
}

void ASGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ASGameState, WaveState);
	DOREPLIFETIME(ASGameState, Team1KillScore);
	DOREPLIFETIME(ASGameState, Team2KillScore);
}