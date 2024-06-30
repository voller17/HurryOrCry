// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseSessionGameInstance.h"

FString UBaseSessionGameInstance::GetPlayerName()
{
	if(PlayerName == FString{"PlayerName"} && TestsPlayerName.Num() > 0)
	{
		const int32 Index = FMath::RandRange(0, TestsPlayerName.Num() - 1);
		return TestsPlayerName[Index];
	}
	return PlayerName;
}

void UBaseSessionGameInstance::SetPlayerName(FString NewName)
{
	PlayerName = NewName;
}