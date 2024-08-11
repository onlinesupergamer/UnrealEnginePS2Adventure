// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interfaces.h"
#include "TestEnemy.generated.h"

UCLASS()
class PS2AD_API ATestEnemy : public ACharacter, public IInterfaces
{
	GENERATED_BODY()

public:
	
	ATestEnemy();
	virtual void Tick(float DeltaTime) override;

	float		Health		=		100.0f;

protected:
	
	virtual void BeginPlay() override;
	virtual void Damage(float Damage) override;
	virtual void BlastDamage(float Damage) override;
	void HealthCheck();
	void KillEnemy();

};
