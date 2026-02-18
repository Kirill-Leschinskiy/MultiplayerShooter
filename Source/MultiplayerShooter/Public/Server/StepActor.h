

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "StepActor.generated.h"

UCLASS()
class MULTIPLAYERSHOOTER_API AStepActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AStepActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void SetupPlayerInput();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Replication")
	float ReplicatedValue;

	// Функция для изменения значения на сервере
	UFUNCTION(BlueprintCallable, Category = "Replication")
	void ChangeValue(float NewValue);

	// Server RPC для изменения значения
	UFUNCTION(Server, Reliable, WithValidation)
	void ServerChangeValue(float NewValue);

	// Необходимо для поддержки репликации
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
};
