// Fill out your copyright notice in the Description page of Project Settings.


#include "TargetComponent.h"
#include "PlayerCharacter.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Interfaces.h"
#include "Kismet/KismetMathLibrary.h"



#define GREEN FColor::Green
#define RED FColor::Red;
#define Blue FColor::blue;

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

	if (m_PlayerCharacter->m_bIsTargeting) 
	{
		if (IsValid(ClosestActor)) 
		{
			GEngine->AddOnScreenDebugMessage(-1, 0.0f, GREEN, TEXT("Found Valid Actor"));

			RotateCamera(ClosestActor);
			FaceTarget(ClosestActor);
		}
		else 
		{
			GEngine->AddOnScreenDebugMessage(-1, 0.0f, GREEN, TEXT("Not Valid Actor"));
		}

	}

}

void UTargetComponent::TargetLockOn() 
{
	SphereTrace();



}

void UTargetComponent::TargetRelease() 
{
	

}

void UTargetComponent::SphereTrace() 
{
	/*
		This simply does a spherecast to check for objects on a specific object channel
		and adds them to an array that can later be processed
	

	*/
	GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Green, TEXT("Starting Sphere Trace"));


	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(m_PlayerCharacter); //If there is nothing in the array, the engine crashes, this is just to stop that
	TArray<FHitResult> HitArray;
	TArray<TEnumAsByte<EObjectTypeQuery>> TypeArray;
	TypeArray.Add(UEngineTypes::ConvertToObjectType(ECC_GameTraceChannel1));

	
	if(UKismetSystemLibrary::SphereTraceMultiForObjects(GetWorld(), m_PlayerCharacter->GetActorLocation(), m_PlayerCharacter->GetActorLocation(),
		LockOnRadius, TypeArray, false, ActorsToIgnore, EDrawDebugTrace::ForDuration, HitArray, true, FColor::Green, FColor::Red, 3.0f))
	{

		
		CheckDistance(HitArray);

	}
 
}

void UTargetComponent::CheckDistance(TArray<FHitResult> m_HitResult)
{
	/*
		This takes the array that was from the spherecast and iterates through them to check which is closest to the camera
	
	
	*/

	float distance = 0.0f;
	float closestDistance = LockOnRadius;
	AActor* m_ClosestActor;
	
	for (const FHitResult &HitArray : m_HitResult) 
	{
		distance = FVector::Dist(m_PlayerCharacter->GetActorLocation(), HitArray.GetActor()->GetActorLocation());

		if (distance < closestDistance) 
		{
			closestDistance = distance;
			m_ClosestActor = HitArray.GetActor();

			if (IsValid(m_ClosestActor))
			{
				//These need to be in TICK since these need to update each frame
				ClosestActor = m_ClosestActor;
				
			}
		}
	}

	GEngine->AddOnScreenDebugMessage(-1, 2.5f, FColor::Green, FString::SanitizeFloat(closestDistance));
}

void UTargetComponent::RotateCamera(AActor* Target) 
{
	//Do Camera Things Here
	GEngine->AddOnScreenDebugMessage(-1, 0.0f, GREEN, TEXT("Rotating Camera"));

	FRotator m_Rot = m_PlayerCharacter->Controller->GetControlRotation();
	FVector m_TargetLocation = ClosestActor->GetActorLocation();
	FRotator m_LookRotation = UKismetMathLibrary::FindLookAtRotation(m_PlayerCharacter->GetActorLocation(), m_TargetLocation);
	m_Rot.Pitch = m_LookRotation.Pitch;
	m_Rot.Yaw = m_LookRotation.Yaw;
	m_Rot = UKismetMathLibrary::RInterpTo(m_PlayerCharacter->GetControlRotation(), m_Rot, GetWorld()->DeltaTimeSeconds, 5.0f);

	m_PlayerCharacter->Controller->SetControlRotation(m_Rot);

}

void UTargetComponent::FaceTarget(AActor* Target) 
{
	//Do Player Rotation Here
	GEngine->AddOnScreenDebugMessage(-1, 0.0f, GREEN, TEXT("Rotating Player"));

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




