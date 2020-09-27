// Fill out your copyright notice in the Description page of Project Settings.


#include "Network/ServerGameSession.h"
#include "Net/UnrealNetwork.h" 
#include "Online.h"
#include "Kismet/GameplayStatics.h"

void AServerGameSession::RegisterServer()
{
	OnCreateSessionCompleteDelegate = FOnCreateSessionCompleteDelegate::CreateUObject(this, &AServerGameSession::OnCreateSessionComplete);
	OnStartSessionCompleteDelegate = FOnStartSessionCompleteDelegate::CreateUObject(this, &AServerGameSession::OnStartOnlineGameComplete);
	
	UWorld* World = GetWorld();
	IOnlineSessionPtr SessionInt = Online::GetSessionInterface();

	FOnlineSessionSettings Settings;
	Settings.NumPublicConnections = 3;
	Settings.bShouldAdvertise = true;
	Settings.bAllowJoinInProgress = true;
	Settings.bIsLANMatch = true;
	Settings.bUsesPresence = true;
	Settings.bAllowJoinViaPresence = true;
	
	
	OnCreateSessionCompleteDelegateHandle = SessionInt->AddOnCreateSessionCompleteDelegate_Handle(OnCreateSessionCompleteDelegate);
	//OnStartSessionCompleteDelegateHandle = SessionInt->AddOnStartSessionCompleteDelegate_Handle(OnStartSessionCompleteDelegate);
	SessionInt->CreateSession(0, FName("Room1"), Settings);
}

void AServerGameSession::OnCreateSessionComplete(FName CreateSessionName, bool bWasSuccessful)
{
	IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get();
	if (OnlineSub)
	{
		IOnlineSessionPtr Sessions = OnlineSub->GetSessionInterface();
		if (Sessions.IsValid())
		{
			Sessions->ClearOnStartSessionCompleteDelegate_Handle(OnStartSessionCompleteDelegateHandle);
		}
	}

	if (bWasSuccessful)
	{
		UGameplayStatics::OpenLevel(GetWorld(), "Meadow_Environment_Set_Map");
	}

	// IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get();
	// 
	// if (OnlineSub)
	// {
	// 	IOnlineSessionPtr Sessions = OnlineSub->GetSessionInterface();
	// 
	// 	if (Sessions.IsValid())
	// 	{
	// 		Sessions->ClearOnCreateSessionCompleteDelegate_Handle(OnCreateSessionCompleteDelegateHandle);
	// 		if (bWasSuccessful)
	// 		{
	// 			OnStartSessionCompleteDelegateHandle = Sessions->AddOnStartSessionCompleteDelegate_Handle(OnStartSessionCompleteDelegate);
	// 
	// 			Sessions->StartSession(CreateSessionName);
	// 		}
	// 	}
	// }
}

void AServerGameSession::OnStartOnlineGameComplete(FName CreateSessionName, bool bWasSuccessful)
{
	IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get();
	if (OnlineSub)
	{
		IOnlineSessionPtr Sessions = OnlineSub->GetSessionInterface();
		if (Sessions.IsValid())
		{
			Sessions->ClearOnStartSessionCompleteDelegate_Handle(OnStartSessionCompleteDelegateHandle);
		}
	}

	if (bWasSuccessful)
	{
		UGameplayStatics::OpenLevel(GetWorld(), "Meadow_Environment_Set_Map", true);
	}
}
