// Fill out your copyright notice in the Description page of Project Settings.


#include "SProjectileWeapon.h"
#include "SBullet.h"
#include "../CoopGame.h"
#include "Kismet/GameplayStatics.h"
#include "PhysicalMaterials/PhysicalMaterial.h"
#include "SHealthComponent.h"
#include "SCharacter.h"
#include "Components/CapsuleComponent.h"

void ASProjectileWeapon::Fire()
{
	if (GetLocalRole() < ROLE_Authority)
	{
		ServerFire();
	}

	if (BulletCount > 0)
	{
		ASCharacter* MyOwner = Cast<ASCharacter>(GetOwner());
		
		FVector MuzzleLocation = MeshComp->GetSocketLocation(MuzzleSocketName);
		FRotator MuzzleRotation = MeshComp->GetSocketRotation(MuzzleSocketName);

		if (MyOwner)
		{
			FVector EyeLocation;
			FRotator EyeRotation;
			MyOwner->GetActorEyesViewPoint(EyeLocation, EyeRotation);
			
			FActorSpawnParameters SpawnParams;
			SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

			ASCharacter* Character = Cast<ASCharacter>(MyOwner);
			ASBullet* BulletActor = Cast<ASBullet>(GetWorld()->SpawnActor<AActor>(ProjectileClass, MuzzleLocation, MuzzleRotation, SpawnParams));

			BulletActor->SetTeamNumber(Character->GetTeamNumber());
			BulletActor->CapsuleComp->OnComponentHit.AddDynamic(this, &ASProjectileWeapon::OnHit);
			BulletCount -= 1;

			PlayFireEffect(MuzzleLocation);
		}
	}
}

void ASProjectileWeapon::ServerFire_Implementation()
{
	Fire();
}

bool ASProjectileWeapon::ServerFire_Validate()
{
	return true;
}

void ASProjectileWeapon::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	AActor* MyOwner = GetOwner();

	if (!USHealthComponent::IsFriendly(MyOwner, OtherActor))
	{
		EPhysicalSurface SurfaceType = UPhysicalMaterial::DetermineSurfaceType(Hit.PhysMaterial.Get());

		float ActualDamage = BaseDamage;

		if (SurfaceType == SURFACE_FLESHVUNERABLE)
		{
			ActualDamage *= 2;
		}

		UGameplayStatics::ApplyDamage(OtherActor, ActualDamage,
			MyOwner->GetInstigatorController(),
			MyOwner, DamageType);


		PlayImpactEffects(SurfaceType, Hit.ImpactPoint);

		if (HasAuthority())
		{
			HitScanTrace.TraceTo = Hit.ImpactPoint;
			HitScanTrace.SurfaceType = SurfaceType;
		}
	}

	AActor* HitOwner = HitComponent->GetOwner();
	UGameplayStatics::ApplyDamage(HitOwner, 101, HitOwner->GetInstigatorController(), HitOwner, nullptr);
}
