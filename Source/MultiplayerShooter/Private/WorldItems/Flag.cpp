


#include "MultiplayerShooter\Public\WorldItems\Flag.h"
#include "Components/StaticMeshComponent.h"
#include "MultiplayerShooterCharacter.h"
#include "GameFramework/Character.h"
#include "Engine/SkeletalMeshSocket.h"
#include "Net/UnrealNetwork.h"

// Sets default values
AFlag::AFlag()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

    bReplicates = true;
    bAlwaysRelevant = true;

    FlagMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("FlagMesh"));
    RootComponent = FlagMesh;
    FlagMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    FlagMesh->SetIsReplicated(true);

    bIsPickedUp = false;
    OwnerCharacter = nullptr;
}

// Called when the game starts or when spawned
void AFlag::BeginPlay()
{
	Super::BeginPlay();
	
}

void AFlag::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(AFlag, OwnerCharacter);
    DOREPLIFETIME(AFlag, bIsPickedUp);
}

void AFlag::PickUp(AMultiplayerShooterCharacter* Character)
{
    if (!HasAuthority()) return; // Только на сервере
    if (!Character || bIsPickedUp) return;

    OwnerCharacter = Character;
    bIsPickedUp = true;

    AttachToCharacter(Character);

    // Подписываемся на смерть персонажа
    if (OwnerCharacter)
    {
        OwnerCharacter->OnDestroyed.AddDynamic(this, &AFlag::OnOwnerDestroyed);
    }
}

void AFlag::Drop()
{
    if (!HasAuthority()) return;

    if (OwnerCharacter)
    {
        // Отписываемся от события смерти
        OwnerCharacter->OnDestroyed.RemoveDynamic(this, &AFlag::OnOwnerDestroyed);

        // Открепляем флаг
        DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
        OwnerCharacter = nullptr;
    }

    bIsPickedUp = false;
}

void AFlag::AttachToCharacter(AMultiplayerShooterCharacter* Character)
{
    if (!Character) return;

    // Получаем сокет на персонаже
    USkeletalMeshComponent* CharacterMesh = Character->GetMesh();
    if (CharacterMesh)
    {
        // Прикрепляем флаг к сокету
        FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, EAttachmentRule::KeepWorld, true);
        AttachToComponent(CharacterMesh, AttachmentRules, FName("flag_socket"));

        // Локальное отключение коллизии
        FlagMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    }
}

void AFlag::OnOwnerDestroyed(AActor* DestroyedActor)
{
    // Если владелец погиб, уничтожаем флаг
    if (HasAuthority() && DestroyedActor == OwnerCharacter)
    {
        Destroy();
    }
}


