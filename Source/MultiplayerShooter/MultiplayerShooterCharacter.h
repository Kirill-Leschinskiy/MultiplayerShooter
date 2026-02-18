// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Delegates/Delegate.h"
#include "Delegates/DelegateCombinations.h"
#include "MultiplayerShooter/Public/Interfaces/ILogger.h"
#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Logging/LogMacros.h"
#include "InputMappingContext.h"
#include "MultiplayerShooterCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UInputAction;
struct FInputActionValue;
class UHealthComponent;
class UShieldComponent;

DECLARE_LOG_CATEGORY_EXTERN(CharacterLog, All, All);


DECLARE_DELEGATE(FSimpleDelegate);

DECLARE_DYNAMIC_DELEGATE(FDynamicDelegate);

UCLASS(abstract)
class AMultiplayerShooterCharacter : public ACharacter, public IILogger
{
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components", meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components", meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FollowCamera;
	
protected:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input") UInputMappingContext* DefaultMappingContext;
	/** Jump Input Action */
	UPROPERTY(EditAnywhere, Category="Input")
	UInputAction* JumpAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, Category="Input")
	UInputAction* MoveAction;

	/** Look Input Action */
	UPROPERTY(EditAnywhere, Category="Input")
	UInputAction* LookAction;

	/** Mouse Look Input Action */
	UPROPERTY(EditAnywhere, Category="Input")
	UInputAction* MouseLookAction;

public:

	/** Constructor */
	AMultiplayerShooterCharacter();	

protected:

	/** Initialize input action bindings */
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void BeginPlay() override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	virtual void LogInfo(TArray<FString>& LogArray) override;

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Components");
	class UHealthComponent* HealthComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Components")
	UShieldComponent* ShieldComponent;
	/** Called for movement input */
	void Move(const FInputActionValue& Value);

	/** Called for looking input */
	void Look(const FInputActionValue& Value);

	
public:

	/** Handles move inputs from either controls or UI interfaces */
	UFUNCTION(BlueprintCallable, Category="Input")
	virtual void DoMove(float Right, float Forward);

	/** Handles look inputs from either controls or UI interfaces */
	UFUNCTION(BlueprintCallable, Category="Input")
	virtual void DoLook(float Yaw, float Pitch);

	/** Handles jump pressed inputs from either controls or UI interfaces */
	UFUNCTION(BlueprintCallable, Category="Input")
	virtual void DoJumpStart();

	/** Handles jump pressed inputs from either controls or UI interfaces */
	UFUNCTION(BlueprintCallable, Category="Input")
	virtual void DoJumpEnd();

	UFUNCTION(BlueprintCallable, Category = "Health")
	UHealthComponent* GetHealthComponent() const { return HealthComponent; }

	UFUNCTION(BlueprintCallable, Category = "Shield")
	UShieldComponent* GetShieldComponent() const { return ShieldComponent; }

	UFUNCTION(BlueprintCallable, Category = "Damage")
	virtual void ApplyDamage(float DamageAmount);
public:
	FSimpleDelegate SimpleDelegate;
	FDynamicDelegate DynamicDelegate;

	UFUNCTION(BlueprintCallable, Category = "Delegates")
	void BlueprintCallableMethod();

	void CppMethod();

	UFUNCTION(BlueprintCallable, Category = "Delegates")
	void ActivateDelegates();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Target Search")
	TSubclassOf<AActor> TargetClass;

	// Первый метод: поиск акторов заданного класса
	UFUNCTION(BlueprintCallable, Category = "Target Search")
	TArray<AActor*> FindActorsOfClass();

	// Второй метод: сортировка массива по дальности (возвращает bool - успех/неудача)
	UFUNCTION(BlueprintCallable, Category = "Target Search")
	bool SortActorsByDistance(TArray<AActor*>& ActorsToSort);

	// Третий метод: BlueprintCallable - вызывает первые два и выводит информацию в лог
	UFUNCTION(BlueprintCallable, Category = "Target Search")
	void FindAndLogTargets();

	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }

	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }
};

