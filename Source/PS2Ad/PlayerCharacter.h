// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "PlayerCharacter.generated.h"




UCLASS()
class PS2AD_API APlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere)
	UCameraComponent* PlayerCamera;

	UPROPERTY(EditAnywhere)
	USpringArmComponent* CameraArm;


	UPROPERTY(EditAnywhere)
	float		LookRate			=		45.0f;
	bool		bIsPlayerFalling	=		false;
	FVector2D	PlayerControllerInput;

	UPROPERTY(BlueprintReadOnly)
	bool		bIsAiming			=		false;
	bool		bIsTransitioning	=		false;
	float		DirectRifleDamage	=		20.0f;
	float		RifleBlastDamage	=		12.0f;
	float		SwordDamage			=		15.0f;
	





	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;



	// Sets default values for this character's properties
	APlayerCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	float		m_Timebffr		=			0.0f;
	float		m_TimerThing	=			0.0f;
	float		m_BlastRadius	=			150.0f;
	float		m_Checkradius	=			500.0f;

	UPROPERTY(BlueprintReadOnly)
	bool		m_bIsTargeting	=			false;
	bool		m_bCanTarget	=			true;
	AActor*		m_Target		=			nullptr;


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
	float QLerp(float f1, float f2, float LerpSpeed);
	void CameraLockToTarget();
	void Target();
	void TargetScan();
	void SwitchCurrentTarget(float Value);

};
