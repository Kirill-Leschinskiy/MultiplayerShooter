// Copyright Epic Games, Inc. All Rights Reserved.

#include "MultiplayerShooterCharacter.h"
#include "Engine/LocalPlayer.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Controller.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "MultiplayerShooter/Public/Components/HealthComponent.h"
#include "MultiplayerShooter/Public/Components/ShieldComponent.h"
#include "MultiplayerShooter.h"

#include "Engine/World.h"
#include "Engine/Engine.h"
#include "EngineUtils.h"

DEFINE_LOG_CATEGORY(CharacterLog);

AMultiplayerShooterCharacter::AMultiplayerShooterCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	PrimaryActorTick.bCanEverTick = false;

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f);

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 500.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f;
	CameraBoom->bUsePawnControlRotation = true;

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named ThirdPersonCharacter (to avoid direct content references in C++)

	HealthComponent = CreateDefaultSubobject<UHealthComponent>("HealthComponent");
	ShieldComponent = CreateDefaultSubobject<UShieldComponent>("ShieldComponent");
}

void AMultiplayerShooterCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {
		
		// Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AMultiplayerShooterCharacter::Move);
		EnhancedInputComponent->BindAction(MouseLookAction, ETriggerEvent::Triggered, this, &AMultiplayerShooterCharacter::Look);

		// Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AMultiplayerShooterCharacter::Look);
	}
	else
	{
		UE_LOG(LogMultiplayerShooter, Error, TEXT("'%s' Failed to find an Enhanced Input component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}

void AMultiplayerShooterCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (APlayerController* PlayerController = Cast<APlayerController>(Controller)) {
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = 
			ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}

	UE_LOG(CharacterLog, Error, TEXT("%hs"), "Hello");

	LogByLogger(FName("CharacterLogWritten"), GetName());

	SimpleDelegate.BindUObject(this, &AMultiplayerShooterCharacter::CppMethod);

	UE_LOG(LogTemp, Warning, TEXT("SimpleDelegate привязан к CppMethod"));
}

void AMultiplayerShooterCharacter::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	OnTakeAnyDamage.Clear();

	Super::EndPlay(EndPlayReason);
}

void AMultiplayerShooterCharacter::LogInfo(TArray<FString>& LogArray)
{
	const auto Loc = GetActorLocation();
	const auto Rot = GetActorRotation();
	LogArray.Add(FString::Printf(TEXT("Character location is X=%f, Y=%f, Z=%f"), Loc.X, Loc.Y, Loc.Z));
	LogArray.Add(FString::Printf(TEXT("Character rotation is X=%f, Y=%f, Z=%f"), Rot.Pitch, Rot.Yaw, Rot.Roll));

	for (UActorComponent* Component : GetComponents()) {
		const auto LogComponent = Cast<IILogger>(Component);
		if (LogComponent) LogComponent->LogByLogger("ComponentLog", Component->GetName());
	}
	
}

void AMultiplayerShooterCharacter::ApplyDamage(float DamageAmount)
{
	if (HealthComponent && IsValid(HealthComponent))
	{
		HealthComponent->TakeDamageWithShield(DamageAmount, ShieldComponent);
	}
}

void AMultiplayerShooterCharacter::BlueprintCallableMethod()
{
	UE_LOG(LogTemp, Warning, TEXT("=== BlueprintCallableMethod вызван! Активирую SimpleDelegate... ==="));

	if (SimpleDelegate.IsBound())
	{
		SimpleDelegate.Execute();
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("SimpleDelegate не привязан!"));
	}
}

void AMultiplayerShooterCharacter::CppMethod()
{
	UE_LOG(LogTemp, Warning, TEXT("=== CppMethod вызван! Активирую DynamicDelegate... ==="));

	if (DynamicDelegate.IsBound())
	{
		DynamicDelegate.ExecuteIfBound();
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("DynamicDelegate не привязан!"));
	}
}

void AMultiplayerShooterCharacter::ActivateDelegates()
{
	UE_LOG(LogTemp, Warning, TEXT("=== ActivateDelegates: Запускаю цепочку делегатов... ==="));

	if (DynamicDelegate.IsBound())
	{
		DynamicDelegate.ExecuteIfBound();
	}
	else if (SimpleDelegate.IsBound())
	{
		SimpleDelegate.Execute();
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Ни один делегат не привязан!"));
	}
}

void AMultiplayerShooterCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	// route the input
	DoMove(MovementVector.X, MovementVector.Y);
}

void AMultiplayerShooterCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	// route the input
	DoLook(LookAxisVector.X, LookAxisVector.Y);
}

void AMultiplayerShooterCharacter::DoMove(float Right, float Forward)
{
	if (GetController() != nullptr)
	{
		// find out which way is forward
		const FRotator Rotation = GetController()->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// add movement 
		AddMovementInput(ForwardDirection, Forward);
		AddMovementInput(RightDirection, Right);
	}
}

void AMultiplayerShooterCharacter::DoLook(float Yaw, float Pitch)
{
	if (GetController() != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(Yaw);
		AddControllerPitchInput(Pitch);
	}
}

void AMultiplayerShooterCharacter::DoJumpStart()
{
	// signal the character to jump
	Jump();
}

void AMultiplayerShooterCharacter::DoJumpEnd()
{
	// signal the character to stop jumping
	StopJumping();
}

TArray<AActor*> AMultiplayerShooterCharacter::FindActorsOfClass()
{
	TArray<AActor*> FoundActors;

	if (!TargetClass)
	{
		UE_LOG(LogTemp, Warning, TEXT("FindActorsOfClass: TargetClass не задан!"));
		return FoundActors;
	}

	UWorld* World = GetWorld();
	if (World)
	{
		for (TActorIterator<AActor> It(World, TargetClass); It; ++It)
		{
			AActor* Actor = *It;
			if (Actor && Actor != this)
			{
				FoundActors.Add(Actor);
			}
		}
	}

	UE_LOG(LogTemp, Warning, TEXT("FindActorsOfClass: Найдено %d акторов класса %s"),
		FoundActors.Num(), *TargetClass->GetName());

	return FoundActors;
}

bool AMultiplayerShooterCharacter::SortActorsByDistance(TArray<AActor*>& ActorsToSort)
{
	if (ActorsToSort.Num() == 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("SortActorsByDistance: Массив пуст!"));
		return false;
	}

	FVector PlayerLocation = GetActorLocation();

	ActorsToSort.Sort([PlayerLocation](const AActor& A, const AActor& B) {
		float DistA = FVector::Dist(PlayerLocation, A.GetActorLocation());
		float DistB = FVector::Dist(PlayerLocation, B.GetActorLocation());
		return DistA > DistB;
		});

	UE_LOG(LogTemp, Warning, TEXT("SortActorsByDistance: Массив отсортирован (по убыванию дальности)"));
	return true;
}

void AMultiplayerShooterCharacter::FindAndLogTargets()
{
	UE_LOG(LogTemp, Warning, TEXT("========== НАЧАЛО ПОИСКА ЦЕЛЕЙ =========="));

	TArray<AActor*> Targets = FindActorsOfClass();

	bool bSortSuccess = SortActorsByDistance(Targets);

	if (!bSortSuccess || Targets.Num() == 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("FindAndLogTargets: Не найдено целей для логирования"));
		return;
	}

	FVector PlayerLocation = GetActorLocation();

	UE_LOG(LogTemp, Warning, TEXT("Найдено и отсортировано %d целей:"), Targets.Num());

	for (int32 i = 0; i < Targets.Num(); i++)
	{
		AActor* Target = Targets[i];
		if (Target)
		{
			FVector TargetLocation = Target->GetActorLocation();
			float Distance = FVector::Dist(PlayerLocation, TargetLocation);

			UE_LOG(LogTemp, Warning, TEXT("  [%d] %s"), i + 1, *Target->GetName());
			UE_LOG(LogTemp, Warning, TEXT("      Дистанция: %.2f"), Distance);
			UE_LOG(LogTemp, Warning, TEXT("      Позиция: X=%.2f, Y=%.2f, Z=%.2f"),
				TargetLocation.X, TargetLocation.Y, TargetLocation.Z);
		}
	}

	UE_LOG(LogTemp, Warning, TEXT("========== КОНЕЦ ПОИСКА ЦЕЛЕЙ =========="));
}
