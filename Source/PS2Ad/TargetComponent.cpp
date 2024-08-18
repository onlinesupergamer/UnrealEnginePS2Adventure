// Fill out your copyright notice in the Description page of Project Settings.


#include "TargetComponent.h"
#include "PlayerCharacter.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetMathLibrary.h"


// Sets default values for this component's properties
UTargetComponent::UTargetComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	m_PlayerCharacter = Cast<APlayerCharacter>(m_PlayerCharacter);
	

	
}


// Called when the game starts
void UTargetComponent::BeginPlay()
{
	Super::BeginPlay();
	

	
	
}


// Called every frame
void UTargetComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	
	
	GEngine->AddOnScreenDebugMessage(-1, 0.0f, FColor::Green, TEXT("Pass"));


	if (ClosestTarget == nullptr) 
	{
		//GEngine->AddOnScreenDebugMessage(-1, 0.0f, FColor::Green, TEXT("Closest Not Found"));

	}

	else 
	{
		//GEngine->AddOnScreenDebugMessage(-1, 0.0f, FColor::Green, TEXT("Closest Found"));

	}

	//return;

	if (m_PlayerCharacter->m_bIsTargeting) 
	{
		if (IsValid(TargetLockedOn)) 
		{
			if (FVector::Dist(m_PlayerCharacter->GetActorLocation(), TargetLockedOn->GetActorLocation()) > LockOnRadius) 
			{
				TargetRelease();
				GEngine->AddOnScreenDebugMessage(-1, 0.0f, FColor::Green, TEXT("Releasing Target"));

			}

			else
			{

				GEngine->AddOnScreenDebugMessage(-1, 0.0f, FColor::Green, TEXT("In Range Target"));

			}
		}
	}
}

void UTargetComponent::TargetLockOn() 
{
	GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, TEXT("Started Targeting From Component"));

	
	SphereTrace();
	CheckDistance();

	if (IsValid(ClosestTarget)) 
	{
		m_PlayerCharacter->m_bIsTargeting = true;
		TargetLockedOn = ClosestTarget;
		//Call Tick Function Here

	}



}

void UTargetComponent::TargetRelease() 
{
	GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, TEXT("Stopped Targeting From Component"));
	

	if (IsValid(ClosestTarget))
	{
		m_PlayerCharacter->m_bIsTargeting = false;

	}

}

void UTargetComponent::SphereTrace() 
{
	/*
		This simply does a spherecast to check for objects on a specific object channel
		and adds them to an array that can later be processed
	

	*/

	TArray<TEnumAsByte<EObjectTypeQuery>> TypeArray;
	TArray<AActor*> Ignore;
	TArray<FHitResult> HitArray;
	TypeArray.Add(UEngineTypes::ConvertToObjectType(ECC_GameTraceChannel1));
	Ignore.Add(m_PlayerCharacter);

	TArray<AActor*> LocalTargets;

	if (UKismetSystemLibrary::SphereTraceMultiForObjects(GetWorld(), m_PlayerCharacter->GetActorLocation(), m_PlayerCharacter->GetActorLocation(),
		LockOnRadius, TypeArray, false, Ignore, EDrawDebugTrace::ForDuration, HitArray, true, FColor::Red, FColor::Yellow, 3.5f)) 
	{
		for (const FHitResult &HitResult : HitArray) 
		{
			
			
			if (UKismetMathLibrary::ClassIsChildOf(HitResult.GetActor()->GetClass(), ActorClassType))
			{
				LocalTargets.Add(HitResult.GetActor());
				GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, TEXT("Adding Enemys To Array"));
				
			}
		}

		Targets = LocalTargets;

		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, TEXT("Sent the Array"));

	}

	else 
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, TEXT("Nothing Found"));

	}
}

void UTargetComponent::CheckDistance() 
{
	GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, TEXT("Starting Check Distance"));

	/*
		This takes the array that was from the spherecast and iterates through them to check which is closest to the camera
	
	
	*/

	TArray<AActor*> Ignore;
	float LocalCompare = 0.0f;
	AActor* LocalTarget;

	for (const AActor* m_Targets : Targets) 
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, TEXT("Starting Loop for Targets"));

		FHitResult m_Hit;

		if (IsValid(m_Targets))
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, TEXT("Targets Are Valid"));


			/*
				This casts a ray out from each element in the array to the player camera to check for line of sight
			*/

			if (UKismetSystemLibrary::LineTraceSingle(GetWorld(), m_PlayerCharacter->PlayerCamera->GetComponentLocation(),
				m_Targets->GetActorLocation(), UEngineTypes::ConvertToTraceType(ECC_Visibility), false, Ignore, EDrawDebugTrace::ForDuration, m_Hit, true, FColor::Red, FColor::Green, 3.0f))
			{
				//////////////////////////////////////

				if (CenterDistanceCheck(m_Hit.GetActor()) < LocalCompare)
				{
					GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, TEXT("Greater Than"));

				}
				else if (CenterDistanceCheck(m_Hit.GetActor()) > LocalCompare)
				{
					//I think this checks the distance of the actor from the forward vector of the player camera?

					LocalCompare = CenterDistanceCheck(m_Hit.GetActor());
					LocalTarget = m_Hit.GetActor();
					ClosestTarget = LocalTarget;

					GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, TEXT("Found Closest Target"));



				}
				else
				{

				}


				GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, TEXT("Firing"));


			}

			else 
			{
				GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, TEXT("Nothing Hit By Ray"));

			}
		}

		else 
		{
			GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, TEXT("Targets not valid"));

		}

		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, TEXT("Ending Function"));

		
	}
}

float UTargetComponent::CenterDistanceCheck(AActor* Target)
{

	if (IsValid(Target)) 
	{
		
		return FVector::DotProduct(UKismetMathLibrary::FindLookAtRotation(Target->GetActorLocation(),
			m_PlayerCharacter->GetActorLocation()).Vector(), m_PlayerCharacter->PlayerCamera->GetForwardVector());
		
	}

	return 0.0f;
}




