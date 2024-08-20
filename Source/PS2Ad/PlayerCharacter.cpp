// Fill out your copyright notice in the Description page of Project Settings.


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
	CameraArm->TargetArmLength = 400.0f;

	m_TargetComponent = CreateDefaultSubobject<UTargetComponent>(TEXT("TargetingComponent"));


	CameraOffset = FVector(0, 35, 75);

	CameraArm->SocketOffset = CameraOffset;
	GetCharacterMovement()->bOrientRotationToMovement = true;


	m_TargetComponent->m_PlayerCharacter = this; // <- Sidenote: I hate that I have do this here

	
}


void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	
}

void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (m_TargetComponent != nullptr) 
	{
		//GEngine->AddOnScreenDebugMessage(-1, 0.0f, FColor::Green, TEXT("FOUND TARGETCOMPONENT"));


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
		AddControllerPitchInput(-Value * LookRate * GetWorld()->GetDeltaSeconds());
	}

}

void APlayerCharacter::LookRight(float Value) 
{
	if (!m_bIsTargeting) 
	{
		AddControllerYawInput(Value * LookRate * GetWorld()->GetDeltaSeconds());
	}
}

void APlayerCharacter::FallCheck() 
{
	bIsPlayerFalling = GetCharacterMovement()->IsFalling();

	if (bIsPlayerFalling) 
	{
		GEngine->AddOnScreenDebugMessage(-1, 0.0f, GREEN, TEXT("Falling"));

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
	/*
		Holy shit, I hate this function
		This took too damn long to implement

	*/


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
		CameraArm->TargetArmLength = QLerp(CameraArm->TargetArmLength, 400.0f, 0.15f);

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
		SetActorRotation(m_NewRot, ETeleportType::None);
	}
}

void APlayerCharacter::FireWeapon() 
{
	if (bIsAiming) 
	{
		FHitResult m_Hit;
		TArray<AActor*> ActorToIgnore;
		ActorToIgnore.Add(this);


		if (UKismetSystemLibrary::SphereTraceSingle(GetWorld(), PlayerCamera->GetComponentLocation(), PlayerCamera->GetComponentLocation() + (PlayerCamera->GetForwardVector() * 2500.0f),
			15.0f, UEngineTypes::ConvertToTraceType(ECC_Visibility), false, ActorToIgnore, EDrawDebugTrace::ForDuration, m_Hit, true, FColor::Red, FColor::Blue, 2.0f))
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
		EDrawDebugTrace::ForDuration, HitArray, true, FColor::Red, FColor::Blue, 3.5f))
	{
		for (const FHitResult &HitResult : HitArray)
		{
			if (IInterfaces* interfaces = Cast<IInterfaces>(HitResult.Actor)) 
			{
				interfaces->BlastDamage(RifleBlastDamage);
			}

			/*
				This is slow so might want to see about fixing this later
				Edit: nvm, I just forgot to use the array as a reference
			
			*/
		
		}

	}
}


void APlayerCharacter::SwordAttack() 
{
	if (!bIsAiming) 
	{
		FVector CheckLocation = GetActorLocation() + (GetActorForwardVector() * 85);

		TArray<FHitResult> HitArray;
		TArray<AActor*> ActorToIgnore;
		ActorToIgnore.Add(this);

		if (UKismetSystemLibrary::SphereTraceMulti(GetWorld(), CheckLocation, CheckLocation, 75, UEngineTypes::ConvertToTraceType(ECC_Visibility), false, ActorToIgnore,
			EDrawDebugTrace::ForDuration, HitArray, true, FColor::Yellow, FColor::Purple, 3.5f)) 
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



