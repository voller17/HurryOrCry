#include "BaseSessionGameState.h"

#include "BaseSessionGameMode.h"
#include "Net/UnrealNetwork.h"

ABaseSessionGameState::ABaseSessionGameState()
{
	WinnerPlayer = nullptr;
	CurrentGamePhase = EGamePhase::WaitingForPlayers;
	TimeRemaining = 30.0f;
}

void ABaseSessionGameState::SetGamePhase(EGamePhase NewPhase)
{
	if (HasAuthority())
	{
		CurrentGamePhase = NewPhase;
		OnRep_CurrentGamePhase();
	}
}

void ABaseSessionGameState::SetGameModeName(FString Name)
{
	if (HasAuthority())
	{
		GameModeName = Name;
	}
}

void ABaseSessionGameState::SetGameModeSlogan(FString Slogan)
{
	if (HasAuthority())
	{
		GameModeSlogan = Slogan;
	}
}

void ABaseSessionGameState::SetTimeRemaining(float NewTime)
{
	if (HasAuthority())
	{
		TimeRemaining = NewTime;
		OnRep_TimeRemaining();
	}
}

void ABaseSessionGameState::OnRep_CurrentGamePhase()
{
	// Обработка изменений фазы игры на клиентах ToDo
	UE_LOG(LogTemp, Log, TEXT("Game Phase Changed: %d"), (uint8)CurrentGamePhase);
	switch (CurrentGamePhase)
	{
		case EGamePhase::WaitingForPlayers:
			OnStartWaitingForPlayersTimer();
		break;
		case EGamePhase::GameModeAnnouncement:
			OnPlayersReady();
		break;	
		case EGamePhase::CharacterSelection:
			OnCharacterSelection();
		break;
		case EGamePhase::PrepareBattle:
			OnStartPrepareBattle();
		break;
		case EGamePhase::Battle:
			OnStartBattle();
		break;
	}
}

void ABaseSessionGameState::OnRep_PlayerStates()
{
	OnPlayerConnectedUpdate.Broadcast();
}

void ABaseSessionGameState::OnRep_TimeRemaining()
{
	TimeRemaining;
	// if TimeRemaining = 0.0f ToDo hide timer
	//ToDo Start local timer phase
}

void ABaseSessionGameState::OnStartWaitingForPlayersTimer()
{
	OnStartWaitingForPlayersDelegate.Broadcast();	
}

void ABaseSessionGameState::AddPlayerState(APlayerState* PlayerState)
{
	UE_LOG(LogTemp, Log, TEXT("ABaseSessionGameState::AddPlayerState NetMode: %d"), GetNetMode());
	Super::AddPlayerState(PlayerState);
	if (GetNetMode() != NM_Client)
	{
		if (ABaseSessionPlayerState* BasePlayerState = Cast<ABaseSessionPlayerState>(PlayerState))
		{
			PlayerStates.AddUnique(BasePlayerState);
		}
	}
	if(HasAuthority())
	{
		OnRep_PlayerStates();
	}
}

void ABaseSessionGameState::RemovePlayerState(APlayerState* PlayerState)
{
	Super::RemovePlayerState(PlayerState);
	if (GetNetMode() != NM_Client)
	{
		if (ABaseSessionPlayerState* BasePlayerState = Cast<ABaseSessionPlayerState>(PlayerState))
		{
			PlayerStates.Remove(BasePlayerState);
		}
	}
    	if (GetNetMode() == NM_Client)
    	{
    		if (!IsValid(PlayerState))
    		{
    			return;
    		}
    
    		if (!PlayerState->HasLocalNetOwner())
    		{
    			//ToDo PopUpMessage
    		}
    	}
}

TArray<ABaseSessionPlayerState*> ABaseSessionGameState::GetPlayers() const
{
	return PlayerStates;
}

bool ABaseSessionGameState::IsWaitingForPlayers() const
{
	return CurrentGamePhase == EGamePhase::WaitingForPlayers;
}

bool ABaseSessionGameState::IsAnnouncingGameMode() const
{
	return CurrentGamePhase == EGamePhase::GameModeAnnouncement;
}

FString ABaseSessionGameState::GetGameModeName() const
{
	return GameModeName;
}

FString ABaseSessionGameState::GetGameModeSlogan() const
{
	return GameModeSlogan;
}

void ABaseSessionGameState::BeginPlay()
{
	Super::BeginPlay();

	GEngine->OnNetworkFailure().AddUObject(this, &ABaseSessionGameState::OnNetworkFailure);
}

void ABaseSessionGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME(ABaseSessionGameState, CurrentGamePhase);
	DOREPLIFETIME(ABaseSessionGameState, TimeRemaining);
	DOREPLIFETIME(ABaseSessionGameState, PlayerStates);
	DOREPLIFETIME_CONDITION(ABaseSessionGameState, GameModeName, COND_InitialOnly);
	DOREPLIFETIME_CONDITION(ABaseSessionGameState, GameModeSlogan, COND_InitialOnly);
}

void ABaseSessionGameState::Multi_OnStartGameModeAnnouncementTimer_Implementation()
{
	OnStartGameModeAnnouncementDelegate.Broadcast();
}

void ABaseSessionGameState::Auth_HandlePlayerVictory(ABaseSessionPlayerState* Player)
{
	Multi_AnnounceVictory(Player);
}

void ABaseSessionGameState::OnWaitingForPlayersTimeOut()
{
	if (OnWaitingForPlayersTimeOutDelegate.IsBound())
	{
		OnWaitingForPlayersTimeOutDelegate.Broadcast();
	}
/*
	const UGameInstance* GameInstance = UGameplayStatics::GetGameInstance(GetWorld());
	if (UBasicPopupManager* PopupManager = GameInstance->GetSubsystem<UBasicPopupManager>())
	{
		PopupManager->PushMessage(FText::FromString(TEXT("Enemy player failed to connect")), 0.5f, 5.0f);
	}
	*///ToDo
	/*
	FTimerHandle TimerHandle;
	const FTimerDelegate Delegate = FTimerDelegate::CreateWeakLambda(this, [&]()
	{
		UE_LOG(LogTemp, Error, TEXT("OnWaitingForPlayersTimeOut::Try restart"));

		if (AGameModeBattleArena* GameMode = GetWorld()->GetAuthGameMode<AGameModeBattleArena>())
		{
			GameMode->RestartGame();
		}
	});
	GetWorldTimerManager().SetTimer(TimerHandle, Delegate, 3.f, false);
	*/
}

void ABaseSessionGameState::OnCharacterSelection()
{
	OnFinishGameModeAnnouncementDelegate.Broadcast();
	OnStartCharacterSelectionDelegate.Broadcast();
}

void ABaseSessionGameState::OnStartPrepareBattle()
{
	OnFinishCharacterSelectionDelegate.Broadcast();
	OnStartPrepareBattleDelegate.Broadcast();
}

void ABaseSessionGameState::OnStartBattle()
{
	if(OnStartBattleDelegate.IsBound())
	{
		OnStartBattleDelegate.Broadcast();	
	}
}

void ABaseSessionGameState::OnPlayersReady()
{
	OnFinishWaitingForPlayersDelegate.Broadcast();
}

void ABaseSessionGameState::CheckPlayersIsReady()
{
	for (const auto PlayerState : PlayerStates)
	{
		if(!PlayerState->IsCharacterSelected())
		{
			return;
		}
	}
	if (ABaseSessionGameMode* GameMode = GetWorld()->GetAuthGameMode<ABaseSessionGameMode>())
	{
		GameMode->FinishCharacterSelection();
	}
}

void ABaseSessionGameState::SetWinnerPlayer(ABaseSessionPlayerState* NewWinnerPlayer)
{
	WinnerPlayer = NewWinnerPlayer;
}

ABaseSessionPlayerState* ABaseSessionGameState::GetWinnerPlayer() const
{
	return WinnerPlayer;
}

void ABaseSessionGameState::Multi_AnnounceVictory_Implementation(ABaseSessionPlayerState* Player)
{
	OnPlayerWinDelegate.Broadcast(Player);
}

void ABaseSessionGameState::OnNetworkFailure(UWorld* World, UNetDriver* NetDriver, ENetworkFailure::Type FailureType, const FString& ErrorMessage)
{
	UE_LOG(LogTemp, Error, TEXT("%s: FailureType = %d  ErrorMessage = %s"), ANSI_TO_TCHAR(__FUNCTION__),
		FailureType, *ErrorMessage);
}
