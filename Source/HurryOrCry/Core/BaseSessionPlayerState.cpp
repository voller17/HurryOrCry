// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseSessionPlayerState.h"

#include "BaseSessionGameInstance.h"
#include "BaseSessionGameState.h"
#include "Net/UnrealNetwork.h"


ABaseSessionPlayerState::ABaseSessionPlayerState()
{
	bIsCharacterSelected = false;
	bReplicates = true;
}

void ABaseSessionPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ABaseSessionPlayerState, SelectedCharacter)
	DOREPLIFETIME(ABaseSessionPlayerState, bIsCharacterSelected)
	DOREPLIFETIME(ABaseSessionPlayerState, PlayerIndex);
}

void ABaseSessionPlayerState::ClientInitialize(AController* C)
{
	Super::ClientInitialize(C);
	SetPlayerName(FString{"PlayerBattleArena"});
}

void ABaseSessionPlayerState::Multi_OnStartBattle_Implementation()
{
	OnBattleStartedDelegate.Broadcast(this);
}
void ABaseSessionPlayerState::OnEndBattle()
{
	OnBattleEndedDelegate.Broadcast(this);
}

void ABaseSessionPlayerState::OnRep_PlayerName()
{
	Super::OnRep_PlayerName();
	if (OnNameChangedDelegate.IsBound())
	{
		OnNameChangedDelegate.Broadcast(this);
	}	
}

void ABaseSessionPlayerState::SetPlayerIndex(int32 Index)
{
	PlayerIndex = Index;
}

void ABaseSessionPlayerState::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}

int32 ABaseSessionPlayerState::GetPlayerIndex() const
{
	return PlayerIndex;
}

TSubclassOf<ABaseCharacter> ABaseSessionPlayerState::GetSelectedCharacter() const
{
	return IsValid(SelectedCharacter) ? SelectedCharacter : nullptr;
}

bool ABaseSessionPlayerState::IsCharacterSelected() const
{
	return bIsCharacterSelected;
}

void ABaseSessionPlayerState::ClientSelectCharacter(const FString& NewSelectedCharacter)
{
	if (const APlayerController* PC = Cast<APlayerController>(GetOwner()); PC && PC->IsLocalController())
	{
		ServerSelectCharacter(NewSelectedCharacter);
	}
}

void ABaseSessionPlayerState::BeginPlay()
{
	Super::BeginPlay();
	if (GetNetMode() != NM_Client)
	{
		if (ABaseSessionGameState* GameState = GetWorld()->GetGameState<ABaseSessionGameState>())
		{
			//GameState->OnTurnTimeOutDelegate.AddUniqueDynamic(this, &ABasePlayerState::OnTurnTimeOut); //ToDo
			GameState->OnStartBattleDelegate.AddUniqueDynamic(this, &ABaseSessionPlayerState::Multi_OnStartBattle); //ToDo
			
		}
	}
	ClientGetCustomPlayerName();
}

void ABaseSessionPlayerState::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	//Settings control parameters from global settings data asset // ToDo
}

void ABaseSessionPlayerState::ServerSelectCharacter_Implementation(const FString& NewSelectedCharacter)
{	
	if (HasAuthority())
	{
		const FSoftClassPath SoftClassPath(NewSelectedCharacter);
		if (UClass* CharacterClass = SoftClassPath.TryLoadClass<ABaseCharacter>())
		{
			SelectedCharacter = CharacterClass;
			bIsCharacterSelected = true;
			UE_LOG(LogTemp, Log, TEXT("Character selected on server: %s"), *SelectedCharacter->GetName());
		}
	}
/*
	if (!NewSelectedCharacter)
	{
		UE_LOG(LogTemp, Log, TEXT("ABaseSessionPlayerState::ServerSelectCharacter_Implementation Is NoValid class"));
		// Здесь можно добавить вашу логику, например, спавн нового персонажа или обновление существующего
	}
	else
	{
		if (HasAuthority())
		{
			SelectedCharacter = NewSelectedCharacter;
			bIsCharacterSelected = true;
			UE_LOG(LogTemp, Log, TEXT("Character class set to: %s"), *SelectedCharacter->GetName());
			UE_LOG(LogTemp, Log, TEXT("Character selected on server: %s"), *SelectedCharacter->GetName());
		}
	}
	*/
}

bool ABaseSessionPlayerState::ServerSelectCharacter_Validate(const FString& NewSelectedCharacter)
{
	const FSoftClassPath SoftClassPath(NewSelectedCharacter);
	const UClass* CharacterClass = SoftClassPath.TryLoadClass<ABaseCharacter>();
	return IsValid(CharacterClass);
}

void ABaseSessionPlayerState::ServerSetCustomPlayerName_Implementation(const FString& NewName)
{
	SetPlayerName(NewName);
}

void ABaseSessionPlayerState::ClientGetCustomPlayerName_Implementation()
{
	if(const UBaseSessionGameInstance* BaseSessionGameInstance = Cast<UBaseSessionGameInstance>(GetGameInstance()))
	{
		ServerSetCustomPlayerName(BaseSessionGameInstance->PlayerName);
	}
}
