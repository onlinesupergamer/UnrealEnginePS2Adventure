// Fill out your copyright notice in the Description page of Project Settings.


#include "TestEnemy.h"


// Sets default values
ATestEnemy::ATestEnemy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ATestEnemy::BeginPlay()
{
	Super::BeginPlay();
	
}

/*
	The damage functions from the interface are redundant
	It may be better to just create a simple function for damage



*/

void ATestEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ATestEnemy::Damage(float Damage) 
{
	Health -= Damage;
	Health = FMath::Clamp(Health, 0.0f, 100.0f);
	HealthCheck();
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, FString::FromInt(Health));
}

void ATestEnemy::BlastDamage(float Damage) 
{
	Health -= Damage;
	Health = FMath::Clamp(Health, 0.0f, 100.0f);
	HealthCheck();
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, FString::FromInt(Health));


}

void ATestEnemy::SwordDamage(float Damage) 
{
	Health -= Damage;
	Health = FMath::Clamp(Health, 0.0f, 100.0f);
	HealthCheck();
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, FString::FromInt(Health));
}

void ATestEnemy::HealthCheck() 
{
	if (Health <= 0) 
	{
		KillEnemy();
	}
}

void ATestEnemy::KillEnemy() 
{
	this->Destroy();
}



