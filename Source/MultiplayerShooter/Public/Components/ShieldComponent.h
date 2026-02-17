

#pragma once
#include "MultiplayerShooter/Public/Interfaces/ILogger.h"
#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ShieldComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MULTIPLAYERSHOOTER_API UShieldComponent : public UActorComponent, public IILogger
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UShieldComponent();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shield")
	float MaxShield = 100.f;

	virtual void BeginPlay() override;

	virtual void LogInfo(TArray<FString>& LogArray) override;

	UFUNCTION(BlueprintCallable)
	void SetShield(float NewShield);

public:	
	UFUNCTION(BlueprintCallable)
	float getCurrentShield() const { return CurrentShield; }

	UFUNCTION(BlueprintCallable)
	float GetMaxShield() const { return MaxShield; }

	UFUNCTION(BlueprintCallable)
	bool IsShieldDepleted() const { return FMath::IsNearlyZero(CurrentShield); }

	float AbsorbDamage(float Damage);

private:
	UPROPERTY(VisibleAnywhere, Category = "Shield")
	float CurrentShield = 0;
	
};
