// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TargetComponent.generated.h"



UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PS2AD_API UTargetComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UTargetComponent();
	float LockOnRadius = 300.0f;
	class APlayerCharacter* m_PlayerCharacter;
	AActor* ClosestActor;



protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	


public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	void TargetLockOn();
	void TargetRelease();
	void SphereTrace();
	void CheckDistance(TArray<FHitResult> HitArray);
	float CenterDistanceCheck(AActor* Target);
	void RotateCamera(AActor* Target);
	void FaceTarget(AActor* Target);
		
};
