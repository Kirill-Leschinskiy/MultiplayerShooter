


#include "MultiplayerShooter\Public\Components\HealthComponent.h"
#include "MultiplayerShooter/Public/Components/ShieldComponent.h"
#include "GameFramework/Actor.h"
#include "Engine/World.h"
#include "GameFramework/DamageType.h"
#include "GameFramework/Controller.h"

UHealthComponent::UHealthComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	bWantsInitializeComponent = true;

}


void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	auto Owner = GetOwner();

	if (Owner && IsValid(Owner)) {
		Owner->OnTakeAnyDamage.AddDynamic(this, &UHealthComponent::TakeDamage);
	}
	SetHealth(MaxHealth);
}

void UHealthComponent::LogInfo(TArray<FString>& LogArray)
{
	LogArray.Add(FString::Printf(TEXT("Max health = %f"), MaxHealth));
	LogArray.Add(FString::Printf(TEXT("Current health = %f"), CurrentHealth));
}

void UHealthComponent::SetHealth(float NewHealth)
{
	const auto ClampHealth = FMath::Clamp(NewHealth, 0.f, MaxHealth);

	CurrentHealth = ClampHealth;

	if (FMath::IsNearlyZero(CurrentHealth) && GetOwner() && IsValid(GetOwner())) GetOwner()->Destroy();

	
}

void UHealthComponent::TakeDamageWithShield(float Damage, UShieldComponent* ShieldComponent)
{
	if (Damage <= 0.f) return;

	float RemainingDamage = Damage;

	if (ShieldComponent && IsValid(ShieldComponent))
	{
		RemainingDamage = ShieldComponent->AbsorbDamage(Damage);
	}

	if (RemainingDamage > 0.f)
	{
		SetHealth(CurrentHealth - RemainingDamage);
	}
}

void UHealthComponent::TakeDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, 
	AController* InstigatedBy, AActor* DamageCauser)
{
	if (Damage <= 0.f) return;

	if (DamagedActor && DamagedActor == GetOwner() && IsValid(this)) {
		UShieldComponent* ShieldComponent = GetOwner()->FindComponentByClass<UShieldComponent>();

		TakeDamageWithShield(Damage, ShieldComponent);
	}

	
}





