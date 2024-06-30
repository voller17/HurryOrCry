// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "Defines.generated.h"

UENUM(BlueprintType)
enum class EGamePhase : uint8
{
	WaitingForPlayers,
	GameModeAnnouncement,
	CharacterSelection,
	PrepareBattle,
	Battle
};

//Delegate signatures
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FSimpleEventSignature);

UCLASS()
class HURRYORCRY_API UDefines : public UObject
{
	GENERATED_BODY()
};
