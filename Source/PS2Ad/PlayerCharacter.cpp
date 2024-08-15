// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "DrawDebugHelpers.h"
#include "Interfaces.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetMathLibrary.h"





#define GREEN FColor::Green




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


	FVector CamOffset(0, 65, 75);

	CameraArm->SocketOffset = CamOffset;
	GetCharacterMovement()->bOrientRotationToMovement = true;

	
}


void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


	FallCheck();
	HandleAiming();
	CameraLockToTarget();
	TargetScan();

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
	PlayerInputComponent->BindAction("LockCamera", IE_Pressed, this, &APlayerCharacter::Target);
	PlayerInputComponent->BindAxis("SwitchTarget", this, &APlayerCharacter::SwitchCurrentTarget);




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
	AddControllerPitchInput(-Value * LookRate * GetWorld()->GetDeltaSeconds());

}

void APlayerCharacter::LookRight(float Value) 
{
	AddControllerYawInput(Value * LookRate * GetWorld()->GetDeltaSeconds());
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
		if (PlayerCamera->FieldOfView > minFOV)
		{
			m_Timebffr += GetWorld()->DeltaTimeSeconds;
			PlayerCamera->FieldOfView = FMath::Clamp(FMath::Lerp(maxFOV, minFOV, m_Timebffr * 8), minFOV, maxFOV);		
		}

		else 
		{
			m_Timebffr = 0.0f;
		}

		GetCharacterMovement()->bOrientRotationToMovement = false;
		CameraArm->TargetArmLength = QLerp(CameraArm->TargetArmLength, 200.0f, 0.15f);

		m_bCanTarget = false;

	}

	if (!bIsAiming)
	{
		if (PlayerCamera->FieldOfView < maxFOV)
		{
			m_Timebffr += GetWorld()->DeltaTimeSeconds;
			PlayerCamera->FieldOfView = FMath::Clamp(FMath::Lerp(minFOV, maxFOV, m_Timebffr * 8), minFOV, maxFOV);
			
		}

		else
		{
			m_Timebffr = 0.0f;

		}

		GetCharacterMovement()->bOrientRotationToMovement = true;
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

		/*
			I Hate ALL of this, WHY IS KISMET LIKE THIS
			
			WTF
		
		*/

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

void APlayerCharacter::CameraLockToTarget() 
{


	if (m_bIsTargeting && IsValid(m_Target) && m_bCanTarget) 
	{


		FRotator m_LookRotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), m_Target->GetActorLocation());
		m_LookRotation.Roll = 0;
		m_LookRotation.Pitch = 0;
		SetActorRotation(m_LookRotation);
		
	
	}

	else if (!IsValid(m_Target)) 
	{
		m_bIsTargeting = false;
		m_Target = nullptr;
		

	}

	if (bIsAiming)
	{
		m_bIsTargeting = false;
		m_Target = nullptr;
		
		//Might be a good idea to automatically retarget when not aiming
	}

}

void APlayerCharacter::TargetScan() 
{
	TArray<AActor*> Ignore;
	AActor* ClosestActor = nullptr;
	TArray<FHitResult> HitArray;
	Ignore.Add(this);
	float ClosestDistance = m_Checkradius;
	float Distance = 0.0f;


	if (!m_bIsTargeting) 
	{
		if (UKismetSystemLibrary::SphereTraceMulti(GetWorld(), GetActorLocation(), GetActorLocation(), m_Checkradius, UEngineTypes::ConvertToTraceType(ECC_Visibility), false, Ignore,
			EDrawDebugTrace::ForDuration, HitArray, true, GREEN, FColor::Red, 0.0f))
		{
			for (int i = 0; i < HitArray.Num(); i++)
			{
				Distance = GetDistanceTo(HitArray[i].GetActor());

				if (Distance < ClosestDistance)
				{
					ClosestDistance = Distance;
					ClosestActor = HitArray[i].GetActor();
					m_Target = ClosestActor;

				}

			}
			if (ClosestActor == nullptr)
			{


			}

		}
	}
}

void APlayerCharacter::Target() 
{
	FVector CamOffset(0, 65, 75);
	FVector NewOffset(0, 0, 75);


	if (!m_bIsTargeting) 
	{
		m_bIsTargeting = true;
		//CameraArm->SocketOffset = CamOffset;

	}

	else
	{
		m_bIsTargeting = false;
		//CameraArm->SocketOffset = NewOffset;
	}
}

void APlayerCharacter::SwitchCurrentTarget(float Value) 
{
	
}


float APlayerCharacter::QLerp(float l1, float l2, float LerpSpeed) 
{
	float f = FMath::Lerp(l1, l2, (1.0f / LerpSpeed) * GetWorld()->DeltaTimeSeconds);
	return f;
}



