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
	UPROPERTY(BlueprintReadWrite)
	FString PlayerName = FString{"CustomPlayerNameKiller"};
};
