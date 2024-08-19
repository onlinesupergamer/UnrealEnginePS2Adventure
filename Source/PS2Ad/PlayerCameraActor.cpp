// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCameraActor.h"

// Sets default values
APlayerCameraActor::APlayerCameraActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	PlayerCamera = CreateDefaultSubobject<ACameraActor>(TEXT("PlayerCamera"));

}

// Called when the game starts or when spawned
void APlayerCameraActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APlayerCameraActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

