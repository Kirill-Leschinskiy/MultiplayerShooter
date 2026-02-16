

#pragma once
#include "MultiplayerShooter/Public/Interfaces/ILogger.h"
#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HealthComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MULTIPLAYERSHOOTER_API UHealthComponent : public UActorComponent, public IILogger
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UHealthComponent();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health");
	float MaxHealth = 300.f;

	virtual void BeginPlay() override;

	virtual void LogInfo(TArray<FString>& LogArray) override;

	UFUNCTION(BlueprintCallable)
	void SetHealth(float NewHealth);
		
private:
	UPROPERTY(VisibleAnywhere, Category = "Health")
	float CurrentHealth = 0;

	UFUNCTION()
	void TakeDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, 
		class AController* InstigatedBy, AActor* DamageCauser);
};
