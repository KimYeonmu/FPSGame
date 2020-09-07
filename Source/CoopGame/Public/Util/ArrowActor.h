// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ArrowActor.generated.h"

class UArrowComponent;

UCLASS()
class COOPGAME_API AArrowActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AArrowActor();

	UPROPERTY(EditDefaultsOnly, Category = "Arrow")
	UArrowComponent* ArrowComp;
};
