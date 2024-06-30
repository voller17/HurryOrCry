// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HurryOrCry/Core/BaseSessionPlayerState.h"
#include "BattleUIWidget.generated.h"

/**
 * 
 */
UCLASS()
class HURRYORCRY_API UBattleUIWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintImplementableEvent)
	void AnnounceBattle(ABaseSessionPlayerState* PlayerState);
};
