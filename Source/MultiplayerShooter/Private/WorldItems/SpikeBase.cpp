


#include "MultiplayerShooter/Public/WorldItems/SpikeBase.h"
#include "MultiplayerShooterCharacter.h"
#include "TimerManager.h"
#include "Engine/World.h"


// Sets default values
ASpikeBase::ASpikeBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

void ASpikeBase::ActivateTrap(AMultiplayerShooterCharacter* Character)
{
    if (!Character || !IsValid(Character))
    {
        return;
    }

    // Наносим урон игроку
    ApplyDamageToCharacter(Character);

    // Устанавливаем флаг, что персонаж на перезарядке
    CharactersOnCooldown.Add(Character, true);

    // Запускаем таймер на перезарядку
    FTimerHandle& TimerHandle = CharacterTimers.FindOrAdd(Character);
    GetWorldTimerManager().SetTimer(TimerHandle,
        FTimerDelegate::CreateUObject(this, &ASpikeBase::CheckAndReactivateTrap, Character),
        CooldownTime, false);
}

void ASpikeBase::ApplyDamageToCharacter(AMultiplayerShooterCharacter* Character)
{
    if (Character && IsValid(Character))
    {
        Character->ApplyDamage(DamageAmount);

        // Можно также добавить визуальные/звуковые эффекты здесь
    }
}

void ASpikeBase::OnCooldownEnd(AMultiplayerShooterCharacter* Character)
{
    if (Character && IsValid(Character))
    {
        // Снимаем флаг перезарядки
        CharactersOnCooldown.Add(Character, false);

        // Очищаем таймер
        if (FTimerHandle* TimerHandle = CharacterTimers.Find(Character))
        {
            GetWorldTimerManager().ClearTimer(*TimerHandle);
            CharacterTimers.Remove(Character);
        }
    }
}

void ASpikeBase::CheckAndReactivateTrap(AMultiplayerShooterCharacter* Character)
{
    if (!Character || !IsValid(Character))
    {
        CharactersOnCooldown.Remove(Character);
        CharacterTimers.Remove(Character);
        return;
    }

    // Проверяем, находится ли персонаж все еще в ловушке
    if (OverlappingCharacters.Contains(Character))
    {
        // Если персонаж все еще в ловушке, активируем ее снова
        ActivateTrap(Character);
    }
    else
    {
        // Если персонаж ушел, просто завершаем перезарядку
        OnCooldownEnd(Character);
    }
}


