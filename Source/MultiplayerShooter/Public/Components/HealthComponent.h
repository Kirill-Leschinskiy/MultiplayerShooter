

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
	UHealthComponent();

	void TakeDamageWithShield(float Damage, class UShieldComponent* ShieldComponent = nullptr);

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health");
	float MaxHealth = 100.f;

	virtual void BeginPlay() override;

	virtual void LogInfo(TArray<FString>& LogArray) override;

	UFUNCTION(BlueprintCallable)
	void SetHealth(float NewHealth);
	
	UFUNCTION(BlueprintCallable)
	float GetCurrentHealth() const { return CurrentHealth; }

	UFUNCTION(BlueprintCallable)
	float GetMaxHealth() const { return MaxHealth; }

	UFUNCTION(BlueprintCallable)
	bool IsDead() const { return FMath::IsNearlyZero(CurrentHealth); }

private:
	UPROPERTY(VisibleAnywhere, Category = "Health")
	float CurrentHealth = 0;

	UFUNCTION()
	void TakeDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, 
		class AController* InstigatedBy, AActor* DamageCauser);
};
