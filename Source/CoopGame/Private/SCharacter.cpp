// Fill out your copyright notice in the Description page of Project Settings.


#include "SCharacter.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/PawnMovementComponent.h"
#include "SWeapon.h"
#include "Components/CapsuleComponent.h"
#include "Components/ArrowComponent.h"
#include "../CoopGame.h"
#include "SHealthComponent.h"
#include "Net/UnrealNetwork.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/UserWidget.h"
#include "Components/PostProcessComponent.h"
#include "GameMode/SGameMode.h"
#include "Ui/InGameBottomUserWidget.h"
#include "SGameState.h"
#include "Ui/ResultUserWidget.h"

// Sets default values
ASCharacter::ASCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComp"));
	SpringArmComp->bUsePawnControlRotation = true;
	SpringArmComp->SetupAttachment(RootComponent);

	ChildArrowComp = CreateDefaultSubobject<UChildActorComponent>(TEXT("ChildArrowComp"));
	ChildArrowComp->SetupAttachment(SpringArmComp);

	GetMovementComponent()->GetNavAgentPropertiesRef().bCanCrouch = true;
	GetCapsuleComponent()->SetCollisionResponseToChannel(COLLISION_WEAPON, ECR_Ignore);

	HealthComp = CreateDefaultSubobject<USHealthComponent>(TEXT("HealthComp"));
	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComp"));
	CameraComp->SetupAttachment(SpringArmComp);

	PostProcessComp = CreateDefaultSubobject<UPostProcessComponent>(TEXT("PostProcessComp"));
	PostProcessComp->SetupAttachment(RootComponent);

	WalkSoundComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("AduioComp"));
	WalkSoundComponent->SetupAttachment(RootComponent);

	ZoomedFOV = 100;
	ZoomInterpSpeed = 20;
	SaveBulletCount = 120;
	DefaultFOV = 200;

	WeaponAttackSocketName = "WeaponSocket";

	bDied = false;
	bFinishFixedZoomTimer = false;
	bFixedZoom = false;
}

// Called when the game starts or when spawned
void ASCharacter::BeginPlay()
{
	Super::BeginPlay();

	DefaultFOV = CameraComp->FieldOfView;
	HealthComp->OnHealthChanged.AddDynamic(this, &ASCharacter::OnHealthChanged);

	WalkSoundComponent->SetSound(WalkSound);

	if (IsLocallyControlled())
	{
		if (KillScoreUserWidgetClass != nullptr)
		{
			UUserWidget* KillWidget = CreateWidget(GetWorld(), KillScoreUserWidgetClass);
			KillWidget->AddToViewport();
		}

		if (IngameBottomUserWidgetClass != nullptr)
		{
			UUserWidget* BottomWidget = CreateWidget(GetWorld(), IngameBottomUserWidgetClass);
			IngameBottomUserWidget = Cast<UInGameBottomUserWidget>(BottomWidget);
			IngameBottomUserWidget->SetMaxHp(100);
			IngameBottomUserWidget->AddToViewport();
		}

		if (ResultWidgetClass != nullptr)
		{
			UUserWidget* Widget = CreateWidget(GetWorld(), ResultWidgetClass);
			ResultUserWidget = Cast<UResultUserWidget>(Widget);
			ResultUserWidget->SetVisibility(ESlateVisibility::Hidden);
			ResultUserWidget->AddToViewport();
		}
		

		ASGameState* GS = GetWorld()->GetGameState<ASGameState>();
		GS->OnChangeGameState.AddDynamic(this, &ASCharacter::FinishGame);
	}

	if (GetLocalRole() == ROLE_Authority)
	{
		FActorSpawnParameters SpawnPArams;
		SpawnPArams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		CurrentWeapon = GetWorld()->SpawnActor<ASWeapon>(StartWeaponClass, FVector::ZeroVector, FRotator::ZeroRotator, SpawnPArams);

		if (CurrentWeapon)
		{
			CurrentWeapon->SetOwner(this);
			CurrentWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, WeaponAttackSocketName);
			CurrentWeapon->OnFinishReload.AddDynamic(this, &ASCharacter::FinishReload);
		}
	}
}

void ASCharacter::MoveForward(float Value)
{
	AddMovementInput(GetActorForwardVector() * Value);
}

void ASCharacter::MoveRight(float Value)
{
	AddMovementInput(GetActorRightVector() * Value);
}

void ASCharacter::BeginCrouch()
{
	Crouch();
}

void ASCharacter::EndCrouch()
{
	UnCrouch();
}

void ASCharacter::BeginZoom()
{
	if (GetLocalRole() < ROLE_Authority)
	{
		ServerBeginZoom();
	}

	if (!bFixedZoom)
	{
		bWantsToZoom = true;
		bFinishFixedZoomTimer = false;
		bFixedZoom = false;

		GetWorldTimerManager().SetTimer(TimerHandle_FixedZoom, this,
			&ASCharacter::FinishFixedZoomTimer,
			0.1f);
	}
}

void ASCharacter::ServerBeginZoom_Implementation()
{
	BeginZoom();
}

bool ASCharacter::ServerBeginZoom_Validate()
{
	return true;
}

void ASCharacter::EndZoom()
{
	if (GetLocalRole() < ROLE_Authority)
	{
		ServerEndZoom();
	}

	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	FViewTargetTransitionParams ViewTargetParams;
	ViewTargetParams.BlendTime = 0.1f;
	ViewTargetParams.bLockOutgoing = false;
	ViewTargetParams.BlendFunction = VTBlend_Linear;

	if (bFixedZoom || bFinishFixedZoomTimer)
	{
		bFixedZoom = false;
		bWantsToZoom = false;
		bFinishFixedZoomTimer = false;

		PlayerController->SetViewTarget(ChildArrowComp->GetChildActor(), ViewTargetParams);
	}
	else
	{
		PlayerController->SetViewTarget(CurrentWeapon->GetChildAimActor(), ViewTargetParams);

		bFixedZoom = true;
	}

	GetWorldTimerManager().ClearTimer(TimerHandle_FixedZoom);
}

void ASCharacter::ServerEndZoom_Implementation()
{
	if (bFixedZoom || bFinishFixedZoomTimer)
	{
		bFixedZoom = false;
		bWantsToZoom = false;
		bFinishFixedZoomTimer = false;
	}
	else
	{
		bFixedZoom = true;
	}
}

bool ASCharacter::ServerEndZoom_Validate()
{
	return true;
}

void ASCharacter::FinishFixedZoomTimer()
{
	bFinishFixedZoomTimer = true;
}

void ASCharacter::OnReq_Weapon()
{
	if (IngameBottomUserWidget != nullptr)
	{
		IngameBottomUserWidget->SetBulletText(CurrentWeapon->GetBulletCount());
		IngameBottomUserWidget->SetSaveBulletText(SaveBulletCount);
	}
}

void ASCharacter::Fire()
{
	if (CurrentWeapon)
	{
		CurrentWeapon->Fire();
	}
}

void ASCharacter::StartFire()
{
	CurrentWeapon->StartFire();
}

void ASCharacter::StopFire()
{
	CurrentWeapon->StopFire();
}

uint8 ASCharacter::GetTeamNumber()
{
	return HealthComp->TeamNum;
}

void ASCharacter::SetTimerOutsideMagneticField(float Damage)
{
	if (IsLocallyControlled())
	{
		GetWorldTimerManager().SetTimer(TimerHandle_InsideMagneticField,
			this,
			&ASCharacter::OnOutsideMagnaticField,
			1,
			true);

		MagnaticFieldDamage = Damage;

		PostProcessComp->bEnabled = true;
	}
}

void ASCharacter::ClearTimerOutsideMagneticField()
{
	GetWorldTimerManager().ClearTimer(TimerHandle_InsideMagneticField);

	PostProcessComp->bEnabled = false;
}

void ASCharacter::OnOutsideMagnaticField()
{
	UGameplayStatics::ApplyDamage(this, MagnaticFieldDamage, GetInstigatorController(),
		this, nullptr);
}

void ASCharacter::Reload()
{
	if (GetLocalRole() < ROLE_Authority)
	{
		ServerReload();
	}

	if (!bReload)
	{
		int AddBullet = CurrentWeapon->GetMaxBulletCount() - CurrentWeapon->GetBulletCount();
		AddBullet = (SaveBulletCount - AddBullet) >= 0 ? AddBullet : SaveBulletCount;

		CurrentWeapon->Reload(AddBullet);

		bReload = true;
	}
}

void ASCharacter::ServerReload_Implementation()
{
	Reload();
}

bool ASCharacter::ServerReload_Validate()
{
	return true;
}

void ASCharacter::FinishReload(int RemoveBulletCount)
{
	bReload = false;
	SaveBulletCount -= RemoveBulletCount;
}

void ASCharacter::FinishGame()
{
	if (ResultUserWidget != nullptr)
	{
		ASGameState* InGameState = GetWorld()->GetGameState<ASGameState>();

		bool IsWin = InGameState->GetWinTeamNumber() == GetTeamNumber();
		ResultUserWidget->SetResult(IsWin);

		ResultUserWidget->SetVisibility(ESlateVisibility::Visible);
	}
}

void ASCharacter::OnHealthChanged(USHealthComponent* OwningHealthComp, float Health, float HealthDelta, const class UDamageType* DamageType,
	class AController* InstigatedBy, AActor* DamageCauser)
{
	if (Health <= 0.0f && !bDied)
	{
		bDied = true;

		ASGameMode* GM = Cast<ASGameMode>(GetWorld()->GetAuthGameMode());

		if (GM)
		{
			GM->OnActorKilled.Broadcast(DamageCauser, this, InstigatedBy);
		}

		WalkSoundComponent->SetActive(false);

		GetMovementComponent()->StopMovementImmediately();
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

		DetachFromControllerPendingDestroy();
		SetLifeSpan(10.0f);
	}

	if (!bAi && IsLocallyControlled())
	{
		IngameBottomUserWidget->SetHp(Health);
	}
}

// Called every frame
void ASCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (IsLocallyControlled())
	{
		float TargetFOV = bWantsToZoom && !bFixedZoom ? ZoomedFOV : DefaultFOV;
		float NewFOV = FMath::FInterpTo(CameraComp->FieldOfView, TargetFOV, DeltaTime, ZoomInterpSpeed);

		SpringArmComp->TargetArmLength = NewFOV;

		CameraComp->SetFieldOfView(NewFOV);

		if (IngameBottomUserWidget != nullptr && CurrentWeapon != nullptr)
		{
			IngameBottomUserWidget->SetBulletText(CurrentWeapon->GetBulletCount());
			IngameBottomUserWidget->SetSaveBulletText(SaveBulletCount);
		}
	}


	if (!WalkSoundComponent->IsPlaying())
	{
		FVector Speed = GetVelocity();

		if (Speed.X >= 1 || Speed.Y >= 1 || Speed.Z >= 1)
		{
			WalkSoundComponent->SetWorldLocation(GetActorLocation());
			WalkSoundComponent->Play();
		}
	}
	else
	{
		FVector Speed = GetVelocity();

		if (Speed.X < 1 && Speed.Y < 1 && Speed.Z < 1)
		{
			WalkSoundComponent->Stop();
		}
	}
}

// Called to bind functionality to input
void ASCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &ASCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ASCharacter::MoveRight);

	PlayerInputComponent->BindAxis("Lookup", this, &ASCharacter::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("Turn", this, &ASCharacter::AddControllerYawInput);

	PlayerInputComponent->BindAction("Crouch", IE_Pressed, this, &ASCharacter::BeginCrouch);
	PlayerInputComponent->BindAction("Crouch", IE_Released, this, &ASCharacter::EndCrouch);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ASCharacter::Jump);

	PlayerInputComponent->BindAction("Zoom", IE_Pressed, this, &ASCharacter::BeginZoom);
	PlayerInputComponent->BindAction("Zoom", IE_Released, this, &ASCharacter::EndZoom);

	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &ASCharacter::StartFire);
	PlayerInputComponent->BindAction("Fire", IE_Released, this, &ASCharacter::StopFire);

	PlayerInputComponent->BindAction("Reload", IE_Pressed, this, &ASCharacter::Reload);
}

FVector ASCharacter::GetPawnViewLocation() const
{
	if (CameraComp)
	{
		return CameraComp->GetComponentLocation();
	}
	
	return Super::GetPawnViewLocation();
}

void ASCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ASCharacter, CurrentWeapon);
	DOREPLIFETIME(ASCharacter, bDied);
	DOREPLIFETIME(ASCharacter, bReload);
	DOREPLIFETIME(ASCharacter, bWantsToZoom);
	DOREPLIFETIME(ASCharacter, bFinishFixedZoomTimer);
	DOREPLIFETIME(ASCharacter, bFixedZoom);
	DOREPLIFETIME(ASCharacter, SaveBulletCount);
}