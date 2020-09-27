// Fill out your copyright notice in the Description page of Project Settings.


#include "Ui/LoginUserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Components/Button.h"
#include "Network/NWGameInstance.h"

void ULoginUserWidget::NativeConstruct()
{
	Super::NativeConstruct();

	CreateButton = Cast<UButton>(GetWidgetFromName(TEXT("CreateBtn")));
	JoinButton = Cast<UButton>(GetWidgetFromName(TEXT("JoinBtn")));

	if (CreateButton != nullptr)
	{
		CreateButton->OnClicked.AddDynamic(this, &ULoginUserWidget::OnClickCreateButton);
	}

	if (JoinButton != nullptr)
	{
		JoinButton->OnClicked.AddDynamic(this, &ULoginUserWidget::OnClickPlayButton);
	}
	
	UNWGameInstance* instance = UNWGameInstance::GetInstance();

	if (instance != nullptr)
	{
		instance->OnFindSessionsCompleteDelegate.BindUFunction(this, FName("OnCompleteFindSession"));
	}
}

void ULoginUserWidget::OnClickCreateButton()
{
	UNWGameInstance* instance = UNWGameInstance::GetInstance();

	instance->DestroySession();

	ULocalPlayer* const Player = instance->GetFirstGamePlayer();
	instance->HostSession(Player->GetPreferredUniqueNetId(), FName("Room1"), true, true, 4);
}

void ULoginUserWidget::OnClickPlayButton()
{
	UNWGameInstance* instance = UNWGameInstance::GetInstance();

	ULocalPlayer* const Player = instance->GetFirstGamePlayer();
	instance->FindSessions(Player->GetPreferredUniqueNetId(), true, true);
}

void ULoginUserWidget::OnCompleteFindSession()
{
	UNWGameInstance* instance = UNWGameInstance::GetInstance();
	instance->JoinOnlineGame();
}

