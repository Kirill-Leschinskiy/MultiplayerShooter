


#include "MultiplayerShooter/Public/PickUp/BasePickUp.h"
#include "Components/SphereComponent.h"


ABasePickUp::ABasePickUp()
{

	PrimaryActorTick.bCanEverTick = false;

	RootComponent = CreateDefaultSubobject<USceneComponent>("RootScene");
	TriggerSphere = CreateDefaultSubobject<USphereComponent>("TriggerSphere");
	TriggerSphere->SetupAttachment(RootComponent);
}


void ABasePickUp::BeginPlay()
{
	Super::BeginPlay();
	
	if (TriggerSphere) {
		TriggerSphere->OnComponentBeginOverlap.AddDynamic(this, &ABasePickUp::ProcessOverlap);
	}
	
}

void ABasePickUp::PickUp(AActor* OtherActor)
{


	if (HasPickedUp) 
	{
		TriggerSphere->OnComponentBeginOverlap.RemoveDynamic(this, &ABasePickUp::ProcessOverlap);
		Destroy();
	}
}

void ABasePickUp::ProcessOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor != this && OtherActor) {
		PickUp(OtherActor);
	}
}




