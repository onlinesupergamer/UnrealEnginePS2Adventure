/*****************************************************************************
**																			**
**																		   	**
**				   Copyright (C) 2024 - All Rights Reserved				   	**
**																			**
******************************************************************************
**																			**
**	Project:		Game													**
**																			**
**	Module:			Player Character			 							**
**																			**
**	Created by:		08/11/2024												**
**																			**
**	Script Author:	Ant														**
**																			**
*****************************************************************************/



#include "PlayerCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "DrawDebugHelpers.h"
#include "Interfaces.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "PlayerCameraActor.h"


#define GREEN FColor::Green
#define RED FColor::Red;
#define BLUE FColor::Blue;


/*
	The damage interface calls could be simplfied



*/


APlayerCharacter::APlayerCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	CameraArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraArm"));
	CameraArm->SetupAttachment(RootComponent);
	CameraArm->bUsePawnControlRotation = true;

	PlayerCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("PlayerCamera"));
	PlayerCamera->SetupAttachment(CameraArm);
	bUseControllerRotationYaw = false;
	CameraArm->TargetArmLength = 275.0f;

	m_TargetComponent = CreateDefaultSubobject<UTargetComponent>(TEXT("TargetingComponent"));


	CameraOffset = FVector(0, 35, 75);

	CameraArm->SocketOffset = CameraOffset;
	GetCharacterMovement()->bOrientRotationToMovement = true;
}

void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (m_TargetComponent == nullptr) 
	{
		GEngine->AddOnScreenDebugMessage(-1, 0.0f, FColor::Green, TEXT("NO TARGET COMPONENT FOUND"));
	}
	FallCheck();
	HandleAiming();
}

void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	PlayerInputComponent->BindAxis("MoveForward", this, &APlayerCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &APlayerCharacter::MoveRight);
	PlayerInputComponent->BindAxis("LookRight", this, &APlayerCharacter::LookRight);
	PlayerInputComponent->BindAxis("LookUp", this, &APlayerCharacter::LookUp);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);
	PlayerInputComponent->BindAction("Aim", IE_Pressed, this, &APlayerCharacter::Aim);
	PlayerInputComponent->BindAction("Aim", IE_Released, this, &APlayerCharacter::StopAim);
	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &APlayerCharacter::FireWeapon);
	PlayerInputComponent->BindAction("GamepadFire", IE_Pressed, this, &APlayerCharacter::FireWeapon);
	PlayerInputComponent->BindAction("SwordAttack", IE_Pressed, this, &APlayerCharacter::SwordAttack);
	PlayerInputComponent->BindAction("Target", IE_Pressed, this, &APlayerCharacter::Target);
}

void APlayerCharacter::MoveForward(float Value) 
{
	FRotator Rotation = Controller->GetControlRotation();
	FRotator Yaw(0, Rotation.Yaw, 0);
	FVector MovementDirection = FRotationMatrix(Yaw).GetUnitAxis(EAxis::X);
	AddMovementInput(MovementDirection, Value);
	PlayerControllerInput.X = Value;
}

void APlayerCharacter::MoveRight(float Value) 
{
	FRotator Rotation = Controller->GetControlRotation();
	FRotator Yaw(0, Rotation.Yaw, 0);
	FVector MovementDirection = FRotationMatrix(Yaw).GetUnitAxis(EAxis::Y);
	AddMovementInput(MovementDirection, Value);
	PlayerControllerInput.Y = Value;
}

void APlayerCharacter::LookUp(float Value) 
{
	if (!m_bIsTargeting) 
	{
		if (bIsAiming) 
		{
			m_CameraInputMultiplier = 0.75f;
		}

		else
		{
			m_CameraInputMultiplier = 1.5f;
		}
		AddControllerPitchInput((-Value * m_CameraInputMultiplier) * LookRate * GetWorld()->GetDeltaSeconds());
	}
}

void APlayerCharacter::LookRight(float Value) 
{
	if (!m_bIsTargeting) 
	{
		if (bIsAiming)
		{
			m_CameraInputMultiplier = 0.75f;
		}

		else
		{
			m_CameraInputMultiplier = 1.5f;
		}
		AddControllerYawInput((Value * m_CameraInputMultiplier)* LookRate * GetWorld()->GetDeltaSeconds());
	}
}

void APlayerCharacter::FallCheck() 
{
	bIsPlayerFalling = GetCharacterMovement()->IsFalling();

	if (bIsPlayerFalling) 
	{
		/*
			This may not be needed
		
		*/
	}
}

void APlayerCharacter::Aim() 
{
	if (!bIsAiming) 
	{
		bIsAiming = true;
	}
}

void APlayerCharacter::StopAim() 
{
	if (bIsAiming)
	{
		bIsAiming = false;
	}
}

void APlayerCharacter::HandleAiming() 
{
	float minFOV = 70.0f;
	float maxFOV = 90.0f;

	if (bIsAiming) 
	{	
		if (m_bIsTargeting) 
		{
			//return;
		}

		if (PlayerCamera->FieldOfView > minFOV)
		{
			FVector m_NewLocation(0, 65, 75);

			m_Timebffr += GetWorld()->DeltaTimeSeconds;
			PlayerCamera->FieldOfView = FMath::Clamp(FMath::Lerp(maxFOV, minFOV, m_Timebffr * 8), minFOV, maxFOV);
			CameraArm->SocketOffset = m_NewLocation;
		}

		else 
		{
			m_Timebffr = 0.0f;
		}
		CameraArm->TargetArmLength = QLerp(CameraArm->TargetArmLength, 200.0f, 0.15f);
		m_bCanTarget = false;
	}

	if (!bIsAiming)
	{
		if (PlayerCamera->FieldOfView < maxFOV)
		{
			FVector m_NewLocation(0, 35, 75);

			m_Timebffr += GetWorld()->DeltaTimeSeconds;
			PlayerCamera->FieldOfView = FMath::Clamp(FMath::Lerp(minFOV, maxFOV, m_Timebffr * 8), minFOV, maxFOV);
			CameraArm->SocketOffset = m_NewLocation;
		}

		else
		{
			m_Timebffr = 0.0f;
		}

		if (m_bIsTargeting)
		{
			GetCharacterMovement()->bOrientRotationToMovement = true;
		}
		CameraArm->TargetArmLength = QLerp(CameraArm->TargetArmLength, 275.0f, 0.15f);
		m_bCanTarget = true;
	}
	CharacterAimRotation(bIsAiming);
}

void APlayerCharacter::CharacterAimRotation(bool bCheck) 
{
	float time = 0.05f;

	//The yaw rotation should be lerped to keep the motion smooth

	FRotator CamRotation = PlayerCamera->GetComponentRotation();
	FRotator CurrentRotation = GetActorRotation();
	CamRotation.Roll = 0;
	CamRotation.Pitch = 0;
	FQuat m_NewRot = FQuat::Slerp(CurrentRotation.Quaternion(), CamRotation.Quaternion(), (1.0f / time) * GetWorld()->DeltaTimeSeconds);
	if (bCheck) 
	{
		SetActorRotation(m_NewRot);
	}
}

void APlayerCharacter::FireWeapon() 
{
	if (bIsAiming) 
	{
		FHitResult m_Hit;
		TArray<AActor*> ActorToIgnore;
		ActorToIgnore.Add(this);

		if (UKismetSystemLibrary::SphereTraceSingle(GetWorld(), PlayerCamera->GetComponentLocation(), PlayerCamera->GetComponentLocation() + (PlayerCamera->GetForwardVector() * 6500.0f),
			15.0f, UEngineTypes::ConvertToTraceType(ECC_Visibility), false, ActorToIgnore, EDrawDebugTrace::None, m_Hit, true, FColor::Red, FColor::Blue, 2.0f))
		{
			if (IInterfaces* interfaces = Cast<IInterfaces>(m_Hit.GetActor())) 
			{
				interfaces->Damage(DirectRifleDamage);
			}
			BlastFire(m_Hit.Location);
		}
	}	
}

void APlayerCharacter::BlastFire(FVector m_HitLocation) 
{
	TArray<FHitResult> HitArray;
	TArray<AActor*> ActorToIgnore;
	ActorToIgnore.Add(this);

	if (UKismetSystemLibrary::SphereTraceMulti(GetWorld(), m_HitLocation, m_HitLocation, m_BlastRadius, UEngineTypes::ConvertToTraceType(ECC_Visibility), false, ActorToIgnore,
		EDrawDebugTrace::None, HitArray, true, FColor::Red, FColor::Blue, 3.5f))
	{
		for (const FHitResult &HitResult : HitArray)
		{
			if (IInterfaces* interfaces = Cast<IInterfaces>(HitResult.Actor)) 
			{
				interfaces->BlastDamage(RifleBlastDamage);
			}
		}
	}
}

void APlayerCharacter::SwordAttack() 
{
	float CurrentDistance;
	AActor* Target = m_TargetComponent->ClosestActor;

	if (!bIsAiming) 
	{
		if (m_bIsTargeting) 
		{
			CurrentDistance = FVector::Dist(GetActorLocation(), Target->GetActorLocation());
			GEngine->AddOnScreenDebugMessage(-1, 2.5f, FColor::Green, FString::FromInt(CurrentDistance));
		}

		FVector CheckLocation = GetActorLocation() + (GetActorForwardVector() * 85);

		TArray<FHitResult> HitArray;
		TArray<AActor*> ActorToIgnore;
		ActorToIgnore.Add(this);

		if (UKismetSystemLibrary::SphereTraceMulti(GetWorld(), CheckLocation, CheckLocation, 75, UEngineTypes::ConvertToTraceType(ECC_Visibility), false, ActorToIgnore,
			EDrawDebugTrace::None, HitArray, true, FColor::Yellow, FColor::Purple, 3.5f)) 
		{
			for (const FHitResult &HitResult : HitArray)
			{
				if (IInterfaces* interfaces = Cast<IInterfaces>(HitResult.Actor))
				{
					interfaces->SwordDamage(SwordDamage);
				}
			}
		}
	}
}

void APlayerCharacter::Target() 
{
	if (m_bIsTargeting) 
	{
		m_TargetComponent->TargetRelease();
		m_bIsTargeting = false;
	}

	else
	{
		m_TargetComponent->TargetLockOn();
		m_bIsTargeting = true;
	}

}

float APlayerCharacter::QLerp(float l1, float l2, float LerpSpeed) 
{
	float f = FMath::Lerp(l1, l2, (1.0f / LerpSpeed) * GetWorld()->DeltaTimeSeconds);
	return f;
}



