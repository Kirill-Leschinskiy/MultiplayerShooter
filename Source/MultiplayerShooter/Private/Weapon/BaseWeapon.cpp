


#include "MultiplayerShooter/Public/Weapon/BaseWeapon.h"


// Sets default values
ABaseWeapon::ABaseWeapon()
{
 	PrimaryActorTick.bCanEverTick = false;

}

void ABaseWeapon::BeginPlay()
{
    Super::BeginPlay();

    int32 I = 100;
}



