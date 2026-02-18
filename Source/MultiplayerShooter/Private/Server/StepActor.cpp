


#include "MultiplayerShooter/public/Server/StepActor.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"
#include "Engine/Engine.h"
#include "Net/UnrealNetwork.h"
#include "Kismet/GameplayStatics.h"
#include "MultiplayerShooterCharacter.h"

DEFINE_LOG_CATEGORY_STATIC(LogStepActor, All, All);

// Sets default values
AStepActor::AStepActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Включаем репликацию для этого актора
	bReplicates = true;

	// Устанавливаем начальное значение
	ReplicatedValue = 0.0f;

}

// Called when the game starts or when spawned
void AStepActor::BeginPlay()
{
	Super::BeginPlay();
	
	if (HasAuthority())
	{
		SetupPlayerInput();
	}
}

void AStepActor::SetupPlayerInput()
{
    // Получаем игрока
    APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
    if (PlayerController)
    {
        // Получаем персонажа
        AMultiplayerShooterCharacter* Character = Cast<AMultiplayerShooterCharacter>(PlayerController->GetPawn());
        if (Character)
        {
            // Здесь можно привязать ввод, но для простоты используем тик
            UE_LOG(LogStepActor, Warning, TEXT("StepActor: Сервер готов к обработке ввода"));
        }
    }
}

// Called every frame
void AStepActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

    // На сервере проверяем нажатие клавиши (для примера используем клавишу 'I')
    if (HasAuthority())
    {
        APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
        if (PlayerController && PlayerController->IsInputKeyDown(FKey("I")))
        {
            // Изменяем значение при нажатии клавиши I
            float NewValue = ReplicatedValue + 1.0f;
            ChangeValue(NewValue);

            UE_LOG(LogStepActor, Warning, TEXT("StepActor: Клавиша I нажата, новое значение = %f"), NewValue);
        }
    }
}

void AStepActor::ChangeValue(float NewValue)
{
    if (HasAuthority())
    {
        // Если мы на сервере, меняем значение напрямую
        ReplicatedValue = NewValue;
        UE_LOG(LogStepActor, Warning, TEXT("StepActor: Значение изменено на сервере: %f"), ReplicatedValue);
    }
    else
    {
        // Если мы на клиенте, вызываем серверную RPC
        ServerChangeValue(NewValue);
    }
}

bool AStepActor::ServerChangeValue_Validate(float NewValue)
{
    // Валидация - можно добавить проверки
    return true;
}

void AStepActor::ServerChangeValue_Implementation(float NewValue)
{
    // Этот метод выполняется на сервере
    ReplicatedValue = NewValue;
    UE_LOG(LogStepActor, Warning, TEXT("StepActor: Server RPC: Значение изменено на %f"), ReplicatedValue);
}

void AStepActor::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    // Указываем, что переменная ReplicatedValue должна реплицироваться
    DOREPLIFETIME(AStepActor, ReplicatedValue);
}
