// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseSessionGameMode.h"
#include "BaseSessionGameState.h"
#include "Defines.h"
#include "Kismet/GameplayStatics.h"

ABaseSessionGameMode::ABaseSessionGameMode()
{
	CurrentGamePhase = EGamePhase::WaitingForPlayers;
	DefaultPawnClass = nullptr;
	DefaultPlayerName = FText::FromString(TEXT("Player"));
}

TArray<APlayerController*> ABaseSessionGameMode::GetPlayingControllers()
{
	return PlayingControllers;
}

void ABaseSessionGameMode::StartPlay()
{
	Super::StartPlay();
}

void ABaseSessionGameMode::EndMatch()
{
	Super::EndMatch();

	if (ServerTerminateTimerHandle.IsValid())
	{
		GetWorldTimerManager().ClearTimer(ServerTerminateTimerHandle);
	}		
	
	if (GetNetMode() == NM_DedicatedServer)
	{
		FTimerHandle Destroy;
		GetWorldTimerManager().SetTimer(Destroy, this, &ABaseSessionGameMode::ServerRequestExit, 2.f);
	}
}

void ABaseSessionGameMode::SetGamePhase(EGamePhase NewPhase)
{
	CurrentGamePhase = NewPhase;

	if (ABaseSessionGameState* BaseSessionGameState = GetGameState<ABaseSessionGameState>())
	{
		BaseSessionGameState->SetGamePhase(NewPhase);
	}

	switch (CurrentGamePhase)
	{
		case EGamePhase::WaitingForPlayers:
			StartWaitingForPlayersTimer();
		break;
		case EGamePhase::GameModeAnnouncement:
			OnPlayersReady();
			break;	
		case EGamePhase::CharacterSelection:
			StartCharacterSelection();
		break;
		case EGamePhase::PrepareBattle:
			StartPrepareBattle();
		break;
		case EGamePhase::Battle:
			StartBattle();
		break;
	}
}

bool ABaseSessionGameMode::IsWaitingForPlayers() const
{
	return PlayingControllers.Num() < MaxPlayers;
}

void ABaseSessionGameMode::StartWaitingForPlayersTimer()
{
	if (bHasGameInitiated)
	{
		return;
	}
	bHasGameInitiated = true;
	GetWorldTimerManager().SetTimer(WaitingForPlayersTimerHandle, this, &ABaseSessionGameMode::OnWaitingForPlayersTimeOut, WaitingForPlayersTimeLimit, false);
	if (ABaseSessionGameState* GameStateBattleArena = GetGameState<ABaseSessionGameState>())
	{
		GameStateBattleArena->SetTimeRemaining(WaitingForPlayersTimeLimit);
		GameStateBattleArena->SetGameModeName(GameModeName);
		GameStateBattleArena->SetGameModeSlogan(GameModeSlogan);
	}
}

void ABaseSessionGameMode::HandleMatchAborted()
{
	Super::HandleMatchAborted();
	EndMatch();
}

bool ABaseSessionGameMode::ReadyToEndMatch_Implementation()
{
	return (IsMatchInProgress() && (PlayingControllers.Num() <= 1));
}

bool ABaseSessionGameMode::ReadyToStartMatch_Implementation()
{
	return PlayingControllers.Num() >= MaxPlayers;//ToDo check
}

void ABaseSessionGameMode::Logout(AController* Exiting)
{
	Super::Logout(Exiting);

	if (APlayerController* PlayerController = Cast<APlayerController>(Exiting); PlayingControllers.Find(PlayerController))
	{
		PlayingControllers.Remove(PlayerController);	
	}
}

bool ABaseSessionGameMode::HasGameStarted() const
{
	return bHasGameStarted;
}

void ABaseSessionGameMode::BeginPlay()
{
	Super::BeginPlay();
	
	SetGamePhase(EGamePhase::WaitingForPlayers);
	
	if (GetNetMode() == NM_DedicatedServer)
	{
		if (ServerTerminateTimerHandle.IsValid())
			GetWorldTimerManager().ClearTimer(ServerTerminateTimerHandle);

		GetWorldTimerManager().SetTimer(ServerTerminateTimerHandle, this, &ABaseSessionGameMode::ServerTerminationCheck, 30.f, true, 30.f);
	}
}

FString ABaseSessionGameMode::InitNewPlayer(APlayerController* NewPlayerController, const FUniqueNetIdRepl& UniqueId, const FString& Options, const FString& Portal)
{
	const FString ErrorMessage = Super::InitNewPlayer(NewPlayerController, UniqueId, Options, Portal);
	
	PlayingControllers.Add(NewPlayerController);
	
	if (IsWaitingForPlayers())
	{
		SetGamePhase(EGamePhase::WaitingForPlayers);
	}
	
	if (PlayingControllers.Num() >= MaxPlayers)
	{
		const FTimerDelegate RespawnDelegate = FTimerDelegate::CreateUObject( this, &ABaseSessionGameMode::SetGamePhase, EGamePhase::GameModeAnnouncement );
		GetWorldTimerManager().SetTimer( GameModeAnnouncementTimerHandle, RespawnDelegate, DelayBeforeStart, false );
	}

	return ErrorMessage;
}

void ABaseSessionGameMode::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);

	auto* GameInstance = UGameplayStatics::GetGameInstance(GetWorld());
/*
	for (TActorIterator<AUnitStartPoint> It(GetWorld()); It; ++It)
	{
		UnitsStartPoints.Add(*It);
	}
	*/ //ToDo init spawn points
}

void ABaseSessionGameMode::OnWaitingForPlayersTimeOut()
{
	UE_LOG(LogTemp, Error, TEXT("OnWaitingForPlayersTimeOut::Try restart"));
	
	if (ABaseSessionGameState* GameStateBattleArena = GetGameState<ABaseSessionGameState>())
	{
		GameStateBattleArena->OnWaitingForPlayersTimeOut();
	}
	RestartGame();
}

void ABaseSessionGameMode::OnPlayersReady()
{
	if (!HasGameStarted())
	{
		GetWorldTimerManager().ClearTimer(WaitingForPlayersTimerHandle);
		if (ABaseSessionGameState* GameStateBattleArena = GetGameState<ABaseSessionGameState>())
		{
			GameStateBattleArena->SetGameModeName(GameModeName);
			GameStateBattleArena->SetGameModeSlogan(GameModeSlogan);
		}
		StartGameModeAnnouncementTimer();
	}
	// destroy all pawns
	for (const auto PlayingController : PlayingControllers)
	{
		if (PlayingController->GetPawn())
		{
			PlayingController->GetPawn()->Destroy();
		}
	}
}

void ABaseSessionGameMode::StartCharacterSelection()
{
	GetWorldTimerManager().SetTimer(CharacterSelectionTimerHandle, this, &ABaseSessionGameMode::FinishCharacterSelection, CharacterSelectedDuration);
	if (ABaseSessionGameState* GameStateBattleArena = GetGameState<ABaseSessionGameState>())
	{
		GameStateBattleArena->SetTimeRemaining(CharacterSelectedDuration);
	}
}

void ABaseSessionGameMode::FinishCharacterSelection()
{
	GetWorldTimerManager().ClearTimer(CharacterSelectionTimerHandle);
	if (ABaseSessionGameState* GameStateBattleArena = GetGameState<ABaseSessionGameState>())
	{
		GameStateBattleArena->SetTimeRemaining(0.0f);
	}
	SetGamePhase(EGamePhase::PrepareBattle);
	bHasGameStarted = true;
}

void ABaseSessionGameMode::StartPrepareBattle()
{
	GetWorldTimerManager().SetTimer(PrepareBattleTimerHandle, this, &ABaseSessionGameMode::FinishPrepareBattle, PrepareBattleDuration);
	if (ABaseSessionGameState* GameStateBattleArena = GetGameState<ABaseSessionGameState>())
	{
		GameStateBattleArena->SetTimeRemaining(PrepareBattleDuration);
	}
	if (ABaseSessionGameState* GameStateBattleArena = GetGameState<ABaseSessionGameState>())
	{
		GameStateBattleArena->SetTimeRemaining(BattleDuration);
		for (const auto PlayingController : PlayingControllers)
		{
			if (const ABaseSessionPlayerState* BaseSessionPlayerState = PlayingController->GetPlayerState<ABaseSessionPlayerState>())
			{
				if(BaseSessionPlayerState->GetSelectedCharacter())
				{
					if (APawn* NewCharacter = GetWorld()->SpawnActor<APawn>(BaseSessionPlayerState->GetSelectedCharacter(), ChoosePlayerStart(PlayingController)->GetActorLocation(), ChoosePlayerStart(PlayingController)->GetActorRotation()))
					{
						PlayingController->Possess(NewCharacter);
					}
				}
			}
		}
	}
}

void ABaseSessionGameMode::FinishPrepareBattle()
{
	GetWorldTimerManager().ClearTimer(PrepareBattleTimerHandle);
	if (ABaseSessionGameState* GameStateBattleArena = GetGameState<ABaseSessionGameState>())
	{
		GameStateBattleArena->SetTimeRemaining(0.0f);
	}
	SetGamePhase(EGamePhase::Battle);
}

void ABaseSessionGameMode::StartBattle()
{
	GetWorldTimerManager().SetTimer(BattleTimerHandle, this, &ABaseSessionGameMode::FinishBattle, BattleDuration);
}

void ABaseSessionGameMode::FinishBattle()
{
	GetWorldTimerManager().ClearTimer(BattleTimerHandle);
	if (ABaseSessionGameState* GameStateBattleArena = GetGameState<ABaseSessionGameState>())
	{
		GameStateBattleArena->SetTimeRemaining(0.0f);
	}
	//ToDo Check finish round and winner
}

void ABaseSessionGameMode::StartGameModeAnnouncementTimer()
{
	GetWorldTimerManager().SetTimer(GameModeAnnouncementTimerHandle, this, &ABaseSessionGameMode::OnGameModeAnnouncementTimeOut, GameModeAnnouncementDuration);
	if (ABaseSessionGameState* GameStateBattleArena = GetGameState<ABaseSessionGameState>())
	{
		GameStateBattleArena->Multi_OnStartGameModeAnnouncementTimer();
		GameStateBattleArena->SetTimeRemaining(0.0f);
	}
}

void ABaseSessionGameMode::OnGameModeAnnouncementTimeOut()
{
	GetWorldTimerManager().ClearTimer(GameModeAnnouncementTimerHandle);
	SetGamePhase(EGamePhase::CharacterSelection);
	if (ABaseSessionGameState* GameStateBattleArena = GetGameState<ABaseSessionGameState>())
	{
		GameStateBattleArena->SetTimeRemaining(0.0f);
	}
}

void ABaseSessionGameMode::ServerTerminationCheck()
{
	if ((PlayingControllers.Num() == 0) && (MatchState != "WaitingToStart"))
		SetMatchState("Aborted");
}

void ABaseSessionGameMode::ServerRequestExit()
{
#if UE_SERVER
	FPlatformMisc::RequestExit(true);
#endif
}
