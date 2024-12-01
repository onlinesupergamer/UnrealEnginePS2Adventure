// /*******************************************************************************																			****																		   	****				   Copyright (C) 2024 - All Rights Reserved				   	****																			**********************************************************************************																			****	Project:			<Project Name>										****																			****	Module:				<Module Name>			 							****																			****	Creation Date:		00/00/00											****																			****	Script Author:		<Your Name>											/*******************************************************************************																			****																		   	****				   Copyright (C) 2024 - All Rights Reserved				   	****																			**********************************************************************************																			****	Project:			<Project Name>										****																			****	Module:				<Module Name>			 							****																			****	Creation Date:		00/00/00											****																			****	Script Author:		<Your Name>											****																			*******************************************************************************/


#include "BGMController.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"


// Sets default values
ABGMController::ABGMController()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ABGMController::BeginPlay()
{
	Super::BeginPlay();
	
	PlayMusic();
}

// Called every frame
void ABGMController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABGMController::PlayMusic() 
{
	if (BGMusic != nullptr)
	{
		UGameplayStatics::PlaySound2D(GetWorld(), BGMusic, 0.35f);
	}
}
