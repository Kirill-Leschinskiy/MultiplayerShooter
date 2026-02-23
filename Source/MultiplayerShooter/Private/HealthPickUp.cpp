


#include "MultiPlayerShooter/Public/PickUp/HealthPickUp.h"
#include "Components/HealthComponent.h"


AHealthPickUp::AHealthPickUp()
{
	PrimaryActorTick.bCanEverTick = false;

}

void AHealthPickUp::BeginPlay()
{
	Super::BeginPlay();
	
}

void AHealthPickUp::PickUp(AActor* OtherActor)
{
	auto HealthComponent = OtherActor->GetComponentByClass<UHealthComponent>();

	if (HealthComponent)
	{
		HasPickedUp = HealthComponent->TryPickUpHealth(HealthPoints);
	}

	Super::PickUp(OtherActor);
}
