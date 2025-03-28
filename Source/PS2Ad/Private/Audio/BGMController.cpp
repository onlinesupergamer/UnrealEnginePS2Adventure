#include "../../Public/Audio/BGMController.h"
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
	if (bPlayBGM) 
	{
		if (bUseFullSongList) 
		{
			if (MusicArray.Num() > 0) 
			{
				int32 SongIndex;
				SongIndex = FMath::RandRange(0, 2);
				UGameplayStatics::PlaySound2D(GetWorld(), MusicArray[SongIndex], MusicVolume);
			}
		}

		else
		{
			if (BGMusic != nullptr) 
			{
				UGameplayStatics::PlaySound2D(GetWorld(), BGMusic, MusicVolume);
			}
			
		}
	}
}
