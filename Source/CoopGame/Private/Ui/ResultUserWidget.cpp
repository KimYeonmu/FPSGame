// Fill out your copyright notice in the Description page of Project Settings.


#include "Ui/ResultUserWidget.h"
#include "Components/TextBlock.h"

void UResultUserWidget::NativeConstruct()
{
	Super::NativeConstruct();

	ResultTextBlock = Cast<UTextBlock>(GetWidgetFromName(TEXT("ResultText")));
}

void UResultUserWidget::SetResult(bool bWin)
{
	if (bWin)
	{
		ResultTextBlock->SetText(FText::FromString("WIN!!!"));
	}
	else
	{
		ResultTextBlock->SetText(FText::FromString("DEFEAT..."));
	}
}
