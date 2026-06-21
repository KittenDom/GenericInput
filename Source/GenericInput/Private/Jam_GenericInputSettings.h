// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine/DeveloperSettings.h"
#include "GameplayTagContainer.h"
#include "InputAction.h"

#include "Jam_GenericInputSettings.generated.h"


UCLASS(Config="GenericInput", DefaultConfig, meta=(DisplayName="Jam - GenericInput"))
class UJam_GenericInputSettings : public UDeveloperSettings
{
	GENERATED_BODY()

public:
	virtual FName GetCategoryName() const override
	{
		return FName(TEXT("Plugins"));
	}

	virtual FName GetSectionName() const override
	{
		return FName(TEXT("Jam Generic Input"));
	}

public:
	UPROPERTY(EditDefaultsOnly, Config, Category="Jam - GenericInput - IMC")
	TSoftObjectPtr<class UInputMappingContext> DefaultMappingContext;

	UPROPERTY(EditDefaultsOnly, Config, Category="Jam - GenericInput")
	TSoftObjectPtr<class UInputAction> MoveAction;

	UPROPERTY(EditDefaultsOnly, Config, Category="Jam - GenericInput")
	TSoftObjectPtr<UInputAction> LookAction;

	UPROPERTY(EditDefaultsOnly, Config, Category="Jam - GenericInput")
	TSoftObjectPtr<UInputAction> JumpAction;
};
