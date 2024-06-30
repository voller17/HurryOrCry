// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "BaseSessionGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class HURRYORCRY_API UBaseSessionGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:

	FString GetPlayerName();
	UFUNCTION(BlueprintCallable)
	void SetPlayerName(FString NewName);
	
protected:
	UPROPERTY()
	FString PlayerName = FString{"PlayerName"};

	UPROPERTY()
	TArray<FString> TestsPlayerName = {TEXT("Tokio"), TEXT("Berlin"), TEXT("Moscow"), TEXT("Helsinki"), TEXT("Oslo")};
};
