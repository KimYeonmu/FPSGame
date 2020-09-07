// Fill out your copyright notice in the Description page of Project Settings.


#include "GameMode/SLoginGameModeBase.h"
#include "Blueprint/UserWidget.h"
#include "Ui/LoginUserWidget.h"
#include "Components/Button.h"

ASLoginGameModeBase::ASLoginGameModeBase()
{

}

void ASLoginGameModeBase::BeginPlay()
{
	UUserWidget* LoginWidget = CreateWidget(GetWorld(), LoginWidgetClass);
	LoginWidget->AddToViewport();
}
