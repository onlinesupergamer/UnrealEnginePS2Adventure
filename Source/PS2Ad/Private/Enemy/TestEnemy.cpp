// Fill out your copyright notice in the Description page of Project Settings.


#include "../../Public/Enemy/TestEnemy.h"
#include "Kismet/GameplayStatics.h"


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
	It may be better to just create a single function for damage



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
	//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, FString::FromInt(Health));
}

void ATestEnemy::BlastDamage(float Damage) 
{
	Health -= Damage;
	Health = FMath::Clamp(Health, 0.0f, 100.0f);
	HealthCheck();
	//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, FString::FromInt(Health));


}

void ATestEnemy::SwordDamage(float Damage) 
{
	Health -= Damage;
	Health = FMath::Clamp(Health, 0.0f, 100.0f);
	HealthCheck();
	//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, FString::FromInt(Health));
}

void ATestEnemy::HealthCheck() 
{
	//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, TEXT("Checking Enemy Health"));

	if (Health <= 0) 
	{
		//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, TEXT("Killing Enemy"));
		

		APlayerCharacter* m_Player = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

		/*
			It could be helpful to cast in the constructor instead of here
		*/

		if (m_Player == nullptr) 
		{
			return;
		}

		bIsAlive = false;
		if (m_Player->m_bIsTargeting)
		{
			if (m_Player->m_TargetComponent->ClosestActor == this)
			{
				m_Player->m_TargetComponent->TargetRelease();
			}
		}
		KillEnemy();
	}
}

void ATestEnemy::KillEnemy() 
{
	this->Destroy();
}





