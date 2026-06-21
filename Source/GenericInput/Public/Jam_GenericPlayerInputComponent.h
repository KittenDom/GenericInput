#pragma once

#include "Components/ActorComponent.h"

#include "Jam_GenericPlayerInputComponent.generated.h"


UCLASS(ClassGroup=(GenericInput), meta = (BlueprintSpawnableComponent))
class GENERICINPUT_API UJam_GenericPlayerInputComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UJam_GenericPlayerInputComponent();

public:
	void InitializeInput(UInputComponent* PlayerInputComponent);
	void DeinitializeInput(UInputComponent* PlayerInputComponent);

private:
	APlayerController* GetOwnerPlayerController() const;
	APawn* GetOwnerPawn() const;

private:
	void HandleMoveInput(const struct FInputActionValue& Value);
	void HandleLookInput(const FInputActionValue& Value);
	void HandleJumpStartInput();
	void HandleJumpEndInput();
};
