// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameSession.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "ServerGameSession.generated.h"

/**
 * 
 */
UCLASS()
class COOPGAME_API AServerGameSession : public AGameSession
{
	GENERATED_BODY()
	
public:
	virtual void RegisterServer() override;

	virtual void OnCreateSessionComplete(FName CreateSessionName, bool bWasSuccessful);

	void OnStartOnlineGameComplete(FName CreateSessionName, bool bWasSuccessful);

	FOnCreateSessionCompleteDelegate OnCreateSessionCompleteDelegate;
	FOnStartSessionCompleteDelegate OnStartSessionCompleteDelegate;

	FDelegateHandle OnCreateSessionCompleteDelegateHandle;
	FDelegateHandle OnStartSessionCompleteDelegateHandle;
};
