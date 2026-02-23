

#pragma once

#include "CoreMinimal.h"
#include "MultiplayerShooter/Public/PickUp/BasePickUp.h"
#include "HealthPickUp.generated.h"

UCLASS()
class MULTIPLAYERSHOOTER_API AHealthPickUp : public ABasePickUp
{
	GENERATED_BODY()
	
public:	
	AHealthPickUp();

protected:
	virtual void BeginPlay() override;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Data")
	float HealthPoints = 50.f;

	virtual void PickUp(AActor* OtherActor) override;
public:	


	
	
};
