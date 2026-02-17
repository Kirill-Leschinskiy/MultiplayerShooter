


#include "MultiplayerShooter/Public/WorldItems/TrapBase.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "MultiplayerShooterCharacter.h"
#include "Engine/World.h"
#include "TimerManager.h"


// Sets default values
ATrapBase::ATrapBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	TriggerBox = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerBox"));
    RootComponent = TriggerBox;
    TriggerBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    TriggerBox->SetCollisionResponseToAllChannels(ECR_Ignore);
    TriggerBox->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
    TriggerBox->SetBoxExtent(FVector(100.f, 100.f, 50.f));

    MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
    MeshComponent->SetupAttachment(RootComponent);
    MeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);

}

void ATrapBase::BeginPlay()
{
	Super::BeginPlay();
	
    if (TriggerBox)
    {
        TriggerBox->OnComponentBeginOverlap.AddDynamic(this, &ATrapBase::OnOverlapBegin);
        TriggerBox->OnComponentEndOverlap.AddDynamic(this, &ATrapBase::OnOverlapEnd);
    }
}

void ATrapBase::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    AMultiplayerShooterCharacter* Character = Cast<AMultiplayerShooterCharacter>(OtherActor);
    if (Character && IsValid(Character))
    {
        OverlappingCharacters.Add(Character);

        if (CanActivateTrap(Character))
        {
            ActivateTrap(Character);
        }
    }
}

void ATrapBase::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
    AMultiplayerShooterCharacter* Character = Cast<AMultiplayerShooterCharacter>(OtherActor);
    if (Character && IsValid(Character))
    {
        OverlappingCharacters.Remove(Character);
    }
}

void ATrapBase::ActivateTrap(AMultiplayerShooterCharacter* Character)
{
    // Базовая реализация ничего не делает
    // Этот метод будет переопределен в дочерних классах
}

bool ATrapBase::CanActivateTrap(AMultiplayerShooterCharacter* Character) const
{
    if (!Character || !IsValid(Character))
    {
        return false;
    }

    // Проверяем, не на перезарядке ли персонаж
    const bool* bIsOnCooldown = CharactersOnCooldown.Find(Character);
    return !(bIsOnCooldown && *bIsOnCooldown);
}


