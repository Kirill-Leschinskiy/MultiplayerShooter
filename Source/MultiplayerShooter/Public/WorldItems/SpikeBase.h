

#pragma once
#include "MultiplayerShooter/Public/WorldItems/TrapBase.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SpikeBase.generated.h"

UCLASS()
class MULTIPLAYERSHOOTER_API ASpikeBase : public ATrapBase
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASpikeBase();

protected:
	// Called when the game starts or when spawned
	virtual void ActivateTrap(AMultiplayerShooterCharacter* Character) override;

	void ApplyDamageToCharacter(AMultiplayerShooterCharacter* Character);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Damage")
	float DamageAmount = 20.0f;

	UFUNCTION()
	void CheckAndReactivateTrap(AMultiplayerShooterCharacter* Character);

	void OnCooldownEnd(AMultiplayerShooterCharacter* Character);

	UPROPERTY()
	TMap<AMultiplayerShooterCharacter*, FTimerHandle> CharacterTimers;

	
	
};
