// Fill out your copyright notice in the Description page of Project Settings.


#include "SGameState.h"
#include "Net/UnrealNetwork.h"

ASGameState::ASGameState()
{
	MaxKillScore = 50;
}

void ASGameState::OnReq_WaveState(EWaveState OldState)
{
	WaveStateChanged(WaveState, OldState);
}

void ASGameState::SetWaveState(EWaveState NewState)
{
	if (HasAuthority())
	{
		EWaveState OldState = WaveState;

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

void ASGameState::SetTeam1KillScore(int Score)
{
	Team1KillScore = Score;
}

void ASGameState::SetTeam2KillScore(int Score)
{
	Team2KillScore = Score;
}

void ASGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ASGameState, WaveState);
	DOREPLIFETIME(ASGameState, Team1KillScore);
	DOREPLIFETIME(ASGameState, Team2KillScore);
}