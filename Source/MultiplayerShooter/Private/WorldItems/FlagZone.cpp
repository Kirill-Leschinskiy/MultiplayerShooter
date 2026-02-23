


#include "MultiPLayerShooter/Public/WorldItems/FlagZone.h"
#include "Components/SphereComponent.h"
#include "MultiplayerShooterCharacter.h"
#include "Public\WorldItems\Flag.h"
#include "Engine/World.h"
#include "Net/UnrealNetwork.h"
#include "TimerManager.h"

AFlagZone::AFlagZone()
{

	PrimaryActorTick.bCanEverTick = false;

    // Включаем репликацию
    bReplicates = true;
    bAlwaysRelevant = true;

    // Настройка триггер сферы
    if (TriggerSphere)
    {
        TriggerSphere->SetSphereRadius(200.0f);
    }

    bFlagAvailable = true;
    Flag = nullptr;
}


void AFlagZone::BeginPlay()
{
	Super::BeginPlay();
	
    if (HasAuthority())
    {
        // Создаем флаг
        SpawnFlag();
    }
}

void AFlagZone::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(AFlagZone, Flag);
    DOREPLIFETIME(AFlagZone, bFlagAvailable);
}

void AFlagZone::SpawnFlag()
{
    if (!HasAuthority()) return;

    FActorSpawnParameters SpawnParams;
    SpawnParams.Owner = this;
    SpawnParams.Instigator = GetInstigator();

    FVector SpawnLocation = GetActorLocation() + FVector(0.0f, 0.0f, 100.0f);
    FRotator SpawnRotation = GetActorRotation();

    Flag = GetWorld()->SpawnActor<AFlag>(AFlag::StaticClass(), SpawnLocation, SpawnRotation, SpawnParams);

    if (Flag)
    {
        Flag->SetOwner(this);
        bFlagAvailable = true;
    }
}

void AFlagZone::PickUp(AActor* OtherActor)
{
    Super::PickUp(OtherActor);

    if (!HasAuthority()) return;
    if (!bFlagAvailable || !Flag) return;

    AMultiplayerShooterCharacter* Character = Cast<AMultiplayerShooterCharacter>(OtherActor);
    if (Character && !Flag->IsPickedUp())
    {
        // Игрок подбирает флаг
        Flag->PickUp(Character);

        if (Flag->IsPickedUp())
        {
            bFlagAvailable = false;

            // Подписываемся на смерть игрока с флагом
            Character->OnDestroyed.AddDynamic(this, &AFlagZone::OnFlagOwnerDied);

            // Отключаем триггер сферу (флаг забрали)
            if (TriggerSphere)
            {
                TriggerSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
            }
        }
    }
}

void AFlagZone::ReturnFlag()
{
    if (!HasAuthority()) return;

    // Уничтожаем старый флаг, если он еще существует
    if (Flag && IsValid(Flag))
    {
        Flag->Destroy();
        Flag = nullptr;
    }

    // Создаем новый флаг
    SpawnFlag();

    // Включаем триггер сферу обратно
    if (TriggerSphere && bFlagAvailable)
    {
        TriggerSphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    }
}

void AFlagZone::OnFlagOwnerDied(AActor* DiedActor)
{
    if (!HasAuthority()) return;

    AMultiplayerShooterCharacter* DiedCharacter = Cast<AMultiplayerShooterCharacter>(DiedActor);
    if (DiedCharacter && Flag && Flag->GetOwnerCharacter() == DiedCharacter)
    {
        // Отписываемся от события смерти
        DiedCharacter->OnDestroyed.RemoveDynamic(this, &AFlagZone::OnFlagOwnerDied);

        // Уничтожаем флаг
        if (Flag && IsValid(Flag))
        {
            Flag->Destroy();
            Flag = nullptr;
        }

        // Запускаем таймер возврата флага
        GetWorldTimerManager().SetTimer(RespawnTimerHandle, this, &AFlagZone::ReturnFlag, RespawnTime, false);
    }
}