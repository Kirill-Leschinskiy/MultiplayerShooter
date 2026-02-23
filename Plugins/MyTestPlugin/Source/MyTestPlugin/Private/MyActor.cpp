// Fill out your copyright notice in the Description page of Project Settings.


#include "MyActor.h"
#include "Engine/Engine.h"

// Sets default values
AMyActor::AMyActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	TickLogInterval = 1.0f;
	TickCounter = 0.0f;
}

// Called when the game starts or when spawned
void AMyActor::BeginPlay()
{
	Super::BeginPlay();
	
    UE_LOG(LogTemp, Warning, TEXT("MyTestActor::BeginPlay() - Actor %s started!"), *GetName());

    // Также можно вывести на экран для наглядности
    if (GEngine)
    {
        GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green,
            FString::Printf(TEXT("MyTestActor %s spawned!"), *GetName()));
    }
}

// Called every frame
void AMyActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

    TickCounter += DeltaTime;

    // Выводим в лог каждые TickLogInterval секунд
    if (TickCounter >= TickLogInterval)
    {
        TickCounter = 0.0f;

        // Вывод в лог
        UE_LOG(LogTemp, Log, TEXT("MyTestActor::Tick() - Actor %s is ticking..."), *GetName());

        // Вывод на экран (опционально)
        if (GEngine)
        {
            GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Yellow,
                FString::Printf(TEXT("MyTestActor %s ticked!"), *GetName()));
        }
    }
}

