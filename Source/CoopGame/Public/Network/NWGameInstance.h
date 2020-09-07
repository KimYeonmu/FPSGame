// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "NWGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class COOPGAME_API UNWGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
	UNWGameInstance(const FObjectInitializer & ObjectInitializer);

public:
	static UNWGameInstance* GetInstance();

public:
	bool HostSession(FUniqueNetIdRepl UserId, FName SessionName,
		bool bIsLAN, bool bIsPresence, int32 MaxNumPlayers); 

	FOnCreateSessionCompleteDelegate OnCreateSessionCompleteDelegate;

	FOnStartSessionCompleteDelegate OnStartSessionCompleteDelegate;

	FDelegateHandle OnCreateSessionCompleteDelegateHandle;
	FDelegateHandle OnStartSessionCompleteDelegateHandle;

	TSharedPtr <class FOnlineSessionSettings> SessionSettings;

	virtual void OnCreateSessionComplete(FName SessionName, bool bWasSuccessful);

	void OnStartOnlineGameComplete(FName SessionName, bool bWasSuccessful);

public:
	void FindSessions(FUniqueNetIdRepl UserId, bool bIsLAN, bool bIsPresence);

	FOnFindSessionsCompleteDelegate OnFindSessionsCompleteDelegate;

	FDelegateHandle OnFindSessionsCompleteDelegateHandle;

	TSharedPtr<class FOnlineSessionSearch> SessionSearch;

	void OnFindSessionsComplete(bool bWasSuccessful);

public:
	void JoinOnlineGame();
	bool JoinSession(FUniqueNetIdRepl UserId, FName SessionName, const FOnlineSessionSearchResult& SearchResult);
	FOnJoinSessionCompleteDelegate OnJoinSessionCompleteDelegate;
	FDelegateHandle OnJoinSessionCompleteDelegateHandle;
	void OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result);

public:
	void DestroySession();

	FOnDestroySessionCompleteDelegate OnDestroySessionCompleteDelegate;
	FDelegateHandle OnDestroySessionCompleteDelegateHandle;
	virtual void OnDestroySessionComplete(FName SessionName, bool bWasSuccessful);
};
