

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TrapBase.generated.h"

class UBoxComponent;
class UStaticMeshComponent;
class AMultiplayerShooterCharacter;

UCLASS()
class MULTIPLAYERSHOOTER_API ATrapBase : public AActor
{
	GENERATED_BODY()
	
public:	
	ATrapBase();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UBoxComponent* TriggerBox;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* MeshComponent;

	virtual void BeginPlay() override;

	UFUNCTION()
	virtual void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	virtual void OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	virtual void ActivateTrap(AMultiplayerShooterCharacter* Character);

	virtual bool CanActivateTrap(AMultiplayerShooterCharacter* Character) const;

	FTimerHandle TrapTimerHandle;

	UPROPERTY()
	TSet<AMultiplayerShooterCharacter*> OverlappingCharacters;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Trap")
	float CooldownTime = 5.0f;

	UPROPERTY()
	TMap<AMultiplayerShooterCharacter*, bool> CharactersOnCooldown;


	
};
