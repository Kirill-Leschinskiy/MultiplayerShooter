

#pragma once

#include "CoreMinimal.h"
#include "MultiplayerShooter/Public/PickUp/BasePickUp.h"
#include "FlagZone.generated.h"

class AFlag;
class AMultiplayerShooterCharacter;

UCLASS()
class MULTIPLAYERSHOOTER_API AFlagZone : public ABasePickUp
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFlagZone();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

    // Переопределяем метод PickUp из базового класса
    virtual void PickUp(AActor* OtherActor) override;

    // Флаг, который принадлежит этой зоне
    UPROPERTY(Replicated, BlueprintReadOnly, Category = "Flag")
    AFlag* Flag;

    // Таймер возврата флага
    FTimerHandle RespawnTimerHandle;

    // Время до респавна флага
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Flag")
    float RespawnTime = 5.0f;

    // Доступен ли флаг для подбора
    UPROPERTY(Replicated, BlueprintReadOnly, Category = "Flag")
    bool bFlagAvailable;

    // Создать флаг
    void SpawnFlag();

    // Возврат флага в зону
    UFUNCTION()
    void ReturnFlag();

    // Обработчик смерти игрока с флагом
    UFUNCTION()
    void OnFlagOwnerDied(AActor* DiedActor);

public:
	
	
};
