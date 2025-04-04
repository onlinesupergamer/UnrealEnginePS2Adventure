#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BGMController.generated.h"

UCLASS()
class PS2AD_API ABGMController : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABGMController();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MusicVolume;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class USoundBase* BGMusic;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<USoundBase*> MusicArray;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bPlayBGM;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bUseFullSongList;

	void PlayMusic();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
