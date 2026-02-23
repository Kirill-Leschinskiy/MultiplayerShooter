


#include "MultiplayerShooter\Public\Components\HealthComponent.h"
#include "MultiplayerShooter/Public/Components/ShieldComponent.h"
#include "Net/UnrealNetwork.h"
#include "GameFramework/Actor.h"
#include "Engine/World.h"
#include "GameFramework/DamageType.h"
#include "GameFramework/Controller.h"

UHealthComponent::UHealthComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	bWantsInitializeComponent = true;

}

void UHealthComponent::InitializeComponent()
{
	Super::InitializeComponent();

	if (!HasAnyFlags(RF_ClassDefaultObject) && GetOwner())
	{
		if (GetOwner()->HasAuthority())
		{
			SetIsReplicated(true);
		}
	}
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

bool UHealthComponent::TryPickUpHealth(float HealthPoints)
{
	if (CurrentHealth == MaxHealth || HealthPoints == 0) {
		return false;
	}
	SetHealth(CurrentHealth + HealthPoints);
	return true;
	
}

void UHealthComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UHealthComponent, CurrentHealth);
}

void UHealthComponent::On_RepCurrentHealth(float NewCurrentHealth)
{
	if (GetOwnerRole()==ROLE_AutonomousProxy && GetNetMode() == NM_Client)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s on local machine"), *GetOwner()->GetName());
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("%s on local machine"), *GetOwner()->GetName());
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





