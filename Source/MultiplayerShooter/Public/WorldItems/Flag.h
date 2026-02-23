

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Flag.generated.h"

class USkeletalMeshComponent;
class UStaticMeshComponent;
class AMultiplayerShooterCharacter;

UCLASS()
class MULTIPLAYERSHOOTER_API AFlag : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFlag();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	// Компоненты
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* FlagMesh;

	// Владелец флага
	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Flag")
	AMultiplayerShooterCharacter* OwnerCharacter;

	// Состояние флага
	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Flag")
	bool bIsPickedUp;

	UFUNCTION()
	void OnOwnerDestroyed(AActor* DestroyedActor);

public:	
	// Подобрать флаг
	UFUNCTION(BlueprintCallable, Category = "Flag")
	void PickUp(AMultiplayerShooterCharacter* Character);

	// Открепить флаг (при смерти)
	UFUNCTION(BlueprintCallable, Category = "Flag")
	void Drop();

	// Прикрепить флаг к игроку
	void AttachToCharacter(AMultiplayerShooterCharacter* Character);

	// Проверка, поднят ли флаг
	bool IsPickedUp() const { return bIsPickedUp; }

	// Получить владельца
	AMultiplayerShooterCharacter* GetOwnerCharacter() const { return OwnerCharacter; }
	
};
