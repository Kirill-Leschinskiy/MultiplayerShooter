

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BasePickUp.generated.h"

UCLASS()
class MULTIPLAYERSHOOTER_API ABasePickUp : public AActor
{
	GENERATED_BODY()
	
public:	
	ABasePickUp();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Trigger")
	class USphereComponent* TriggerSphere;

	virtual void PickUp(AActor* OtherActor);

	bool HasPickedUp = false;
public:	

private:
	UFUNCTION()
	void ProcessOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	
	
};
