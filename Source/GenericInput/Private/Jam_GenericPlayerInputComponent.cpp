#include "Jam_GenericPlayerInputComponent.h"

#include "Jam_GenericInputSettings.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputAction.h"
#include "InputMappingContext.h"
#include "Jam_GenericInputLog.h"
#include "GameFramework/Character.h"
#include "GameFramework/PlayerController.h"


UJam_GenericPlayerInputComponent::UJam_GenericPlayerInputComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UJam_GenericPlayerInputComponent::InitializeInput(UInputComponent* PlayerInputComponent)
{
	if (!PlayerInputComponent)
	{
		return;
	}

	const APlayerController* PlayerController = GetOwnerPlayerController();
	if (!PlayerController)
	{
		UE_LOG(Log_JamGenericInput, Warning, TEXT("Owner has no PlayerController. %hs"), __FUNCTION__);
		return;
	}

	const UJam_GenericInputSettings* InputSetting = GetDefault<UJam_GenericInputSettings>();
	if (!InputSetting)
	{
		UE_LOG(Log_JamGenericInput, Warning, TEXT("GenericInput settings is invalid. %hs"), __FUNCTION__);
		return;
	}

	UInputMappingContext* DefaultIMC = InputSetting->DefaultMappingContext.LoadSynchronous();
	if (!DefaultIMC)
	{
		UE_LOG(Log_JamGenericInput, Warning, TEXT("DefaultMappingContext not set in GenericInput settings. %hs"), __FUNCTION__);
		return;
	}

	UEnhancedInputLocalPlayerSubsystem* EnhancedInputSystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer());
	if (!EnhancedInputSystem)
	{
		UE_LOG(Log_JamGenericInput, Warning, TEXT("EnhancedInputLocalPlayerSubsystem is invalid. %hs"), __FUNCTION__);
		return;
	}
	EnhancedInputSystem->AddMappingContext(DefaultIMC, 0);

	UEnhancedInputComponent* EIC = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	if (!EIC)
	{
		UE_LOG(Log_JamGenericInput, Warning, TEXT("InputComponent is not UEnhancedInputComponent. %hs"), __FUNCTION__);
		return;
	}

	if (UInputAction* MoveAction = InputSetting->MoveAction.LoadSynchronous())
	{
		EIC->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ThisClass::HandleMoveInput);
	}

	if (UInputAction* LookAction = InputSetting->LookAction.LoadSynchronous())
	{
		EIC->BindAction(LookAction, ETriggerEvent::Triggered, this, &ThisClass::HandleLookInput);
	}

	if (UInputAction* JumpAction = InputSetting->JumpAction.LoadSynchronous())
	{
		EIC->BindAction(JumpAction, ETriggerEvent::Started, this, &ThisClass::HandleJumpStartInput);
		EIC->BindAction(JumpAction, ETriggerEvent::Completed, this, &ThisClass::HandleJumpEndInput);
	}
}

void UJam_GenericPlayerInputComponent::DeinitializeInput(UInputComponent* PlayerInputComponent)
{
	if (!PlayerInputComponent)
	{
		return;
	}

	UEnhancedInputComponent* EIC = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	if (!EIC)
	{
		return;
	}
	EIC->ClearBindingsForObject(this);
}

APlayerController* UJam_GenericPlayerInputComponent::GetOwnerPlayerController() const
{
	if (APlayerController* PlayerController = Cast<APlayerController>(GetOwner()))
	{
		return PlayerController;
	}

	if (APawn* Pawn = Cast<APawn>(GetOwner()))
	{
		return Cast<APlayerController>(Pawn->GetController());
	}

	return nullptr;
}

APawn* UJam_GenericPlayerInputComponent::GetOwnerPawn() const
{
	if (APawn* Pawn = Cast<APawn>(GetOwner()))
	{
		return Pawn;
	}

	if (APlayerController* PlayerController = Cast<APlayerController>(GetOwner()))
	{
		return PlayerController->GetPawn();
	}

	return nullptr;
}

void UJam_GenericPlayerInputComponent::HandleMoveInput(const FInputActionValue& Value)
{
	const FVector2D Axis = Value.Get<FVector2D>();
	if (Axis.IsNearlyZero())
	{
		return;
	}

	APawn* OwnerPawn = GetOwnerPawn();
	if (!OwnerPawn)
	{
		return;
	}

	const AController* PawnController = OwnerPawn->GetController();
	if (!PawnController)
	{
		return;
	}

	const FRotator YawRotation(0.f, PawnController->GetControlRotation().Yaw, 0.f);
	const FVector Forward = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector Right = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	OwnerPawn->AddMovementInput(Forward, Axis.Y);
	OwnerPawn->AddMovementInput(Right, Axis.X);
}

void UJam_GenericPlayerInputComponent::HandleLookInput(const FInputActionValue& Value)
{
	const FVector2D Axis = Value.Get<FVector2D>();
	APawn* OwnerPawn = GetOwnerPawn();
	if (!OwnerPawn)
	{
		return;
	}

	OwnerPawn->AddControllerYawInput(Axis.X);
	OwnerPawn->AddControllerPitchInput(Axis.Y);
}

void UJam_GenericPlayerInputComponent::HandleJumpStartInput()
{
	ACharacter* Character = Cast<ACharacter>(GetOwnerPawn());
	if (!Character)
	{
		return;
	}

	Character->Jump();
}

void UJam_GenericPlayerInputComponent::HandleJumpEndInput()
{
	ACharacter* Character = Cast<ACharacter>(GetOwnerPawn());
	if (!Character)
	{
		return;
	}

	Character->StopJumping();
}
