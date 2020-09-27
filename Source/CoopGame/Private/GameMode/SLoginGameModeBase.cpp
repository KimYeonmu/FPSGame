// Fill out your copyright notice in the Description page of Project Settings.


#include "GameMode/SLoginGameModeBase.h"
#include "Blueprint/UserWidget.h"
#include "Ui/LoginUserWidget.h"
#include "Components/Button.h"
#include "Network/NWGameInstance.h"
#include "Net/UnrealNetwork.h" 
#include "Online.h"
#include "Net/OnlineEngineInterface.h"
#include "GameFramework/GameSession.h"
#include "Network/ServerGameSession.h"

ASLoginGameModeBase::ASLoginGameModeBase()
{

}

void ASLoginGameModeBase::BeginPlay()
{
	if (GetWorld()->GetNetMode() != NM_DedicatedServer)
	{
		UUserWidget* LoginWidget = CreateWidget(GetWorld(), LoginWidgetClass);
		LoginWidget->AddToViewport();
	}
	else
	{
		//UNWGameInstance* instance = UNWGameInstance::GetInstance();
		//instance->DestroySession();
		//
		//ULocalPlayer* const Player = instance->GetFirstGamePlayer();
		//instance->HostSession(Player->GetPreferredUniqueNetId(), FName("Room1"), true, true, 4);
	}
}

TSubclassOf<AGameSession> ASLoginGameModeBase::GetGameSessionClass() const
{
	return AServerGameSession::StaticClass();
}
