// Fill out your copyright notice in the Description page of Project Settings.


#include "Util/ArrowActor.h"
#include "Components/ArrowComponent.h"

// Sets default values
AArrowActor::AArrowActor()
{
	ArrowComp = CreateDefaultSubobject<UArrowComponent>(TEXT("ArrowComp"));
	SetRootComponent(ArrowComp);
}

