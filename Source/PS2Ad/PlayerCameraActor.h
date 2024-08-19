// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/GameplayStatics.h"
#include "Camera/CameraActor.h"
#include "PlayerCameraActor.generated.h"


UCLASS()
class PS2AD_API APlayerCameraActor : public AActor
{
	GENERATED_BODY()

	
	
public:	
	// Sets default values for this actor's properties
	APlayerCameraActor();

	ACameraActor* PlayerCamera;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
