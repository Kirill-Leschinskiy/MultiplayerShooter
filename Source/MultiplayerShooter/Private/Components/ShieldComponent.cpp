


#include "MultiplayerShooter/Public/Components/ShieldComponent.h"



UShieldComponent::UShieldComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

}



void UShieldComponent::BeginPlay()
{
	Super::BeginPlay();
	
	SetShield(MaxShield);
}

void UShieldComponent::LogInfo(TArray<FString>& LogArray)
{
    LogArray.Add(FString::Printf(TEXT("Max shield = %f"), MaxShield));
    LogArray.Add(FString::Printf(TEXT("Current shield = %f"), CurrentShield));
}

void UShieldComponent::SetShield(float NewShield)
{
    CurrentShield = FMath::Clamp(NewShield, 0.f, MaxShield);
}

float UShieldComponent::AbsorbDamage(float Damage)
{
    if (Damage <= 0.f || IsShieldDepleted())
    {
        return Damage;
    }

    float RemainingDamage = Damage;

    if (CurrentShield >= Damage)
    {
        SetShield(CurrentShield - Damage);
        return 0.f;
    }
    else
    {
        RemainingDamage = Damage - CurrentShield;
        SetShield(0.f);
        return RemainingDamage;
    }
}