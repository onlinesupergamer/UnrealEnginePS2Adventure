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

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "../../Public/Components/TargetComponent.h"
#include "PlayerCharacter.generated.h"




UCLASS()
class PS2AD_API APlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UCameraComponent*		PlayerCamera;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USpringArmComponent*	CameraArm;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UTargetComponent* m_TargetComponent;
	
	float		LookRate				= 45.0f;
	bool		bIsPlayerFalling		= false;
	FVector2D	PlayerControllerInput;

	UPROPERTY(BlueprintReadWrite)
	float		PlayerHealth			= 100.0f;

	UPROPERTY(BlueprintReadOnly)
	bool		bIsAiming				= false;
	bool		bIsTransitioning		= false;
	float		DirectRifleDamage		= 20.0f;
	float		RifleBlastDamage		= 12.0f;
	float		SwordDamage				= 15.0f;
	FVector		CameraOffset;
	TArray<FHitResult> TargetArray;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	bool		m_bIsTargeting			= false;
	UPROPERTY(BlueprintReadWrite)
	AActor*		m_Target				= nullptr;



	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;



	// Sets default values for this character's properties
	APlayerCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	float		m_Timebffr				= 0.0f;
	float		m_TimerThing			= 0.0f;
	float		m_BlastRadius			= 75.0f;
	float		m_Checkradius			= 500.0f;
	float		m_CameraInputMultiplier = 1.0f;
	bool		m_bCanTarget			= true;

	


	void MoveForward(float Value);
	void MoveRight(float Value);
	void LookUp(float Value);
	void LookRight(float Value);
	void FallCheck();
	void Aim();
	void StopAim();
	void HandleAiming();
	void CharacterAimRotation(bool bCheck);
	void FireWeapon();
	void SwordAttack();
	void BlastFire(FVector HitLocation);
	void Target();
	float QLerp(float f1, float f2, float LerpSpeed);
};