// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseSessionPlayerController.h"


ABaseSessionPlayerController::ABaseSessionPlayerController()
{
	//CheatClass = UBaseCheatManager::StaticClass(); ToDo
}

void ABaseSessionPlayerController::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
}

void ABaseSessionPlayerController::BeginPlay()
{
	Super::BeginPlay();
	RegisterConsoleVariables();
	SetPlayerState(GetPlayerState<ABaseSessionPlayerState>());
}

void ABaseSessionPlayerController::OnPossess(APawn* aPawn)
{
	Super::OnPossess(aPawn);//ToDo
}

/*
void APlayerControllerBattleArena::AddCheats(bool bForce)
{
	// (IsValid(CheatManager))
	//	CheatManager->BeginDestroy();

	if (const AGameStateBattleArena* BaseGameState = Cast<AGameStateBattleArena>(GetWorld()->GetGameState()))
	{
		if (BaseGameState->bAllowCheatManager)
		{
			CheatManager = NewObject<UCheatManager>(this,CheatClass);
			CheatManager->InitCheatManager();
		}
	}
}

void APlayerControllerBattleArena::EnableCheats()
{
	AddCheats(true);
}
*/

void ABaseSessionPlayerController::InitPlayerState()
{
	Super::InitPlayerState();
}

void ABaseSessionPlayerController::PostInitializeComponents()
{
	Super::PostInitializeComponents();
}
void ABaseSessionPlayerController::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();
	AddCheats();
}

void ABaseSessionPlayerController::SetPlayerState(ABaseSessionPlayerState* NewPlayerState)
{
	if (GetNetMode() != NM_Client)
	{
		if (IsValid(NewPlayerState) && NewPlayerState != BasePlayerState)
		{
			BasePlayerState = NewPlayerState;
			BasePlayerState->OnBattleStartedDelegate.AddUniqueDynamic(this, &ABaseSessionPlayerController::Multi_OnBattleStarted);
			BasePlayerState->OnBattleEndedDelegate.AddUniqueDynamic(this, &ABaseSessionPlayerController::Multi_OnBattleEnded);
			BasePlayerState->OnBattleTimeOutDelegate.AddUniqueDynamic(this, &ABaseSessionPlayerController::Multi_OnBattleTimeOut);
		}
	}	
}

void ABaseSessionPlayerController::ClientSetHUD_Implementation(TSubclassOf<AHUD> NewHUDClass)
{
	/*
	const APlayerControllerBattleArena* MainPlayerController = Cast<APlayerControllerBattleArena>(
		UGameplayStatics::GetPlayerController(this, 0));

	if (MainPlayerController != this)
	{
		return;
	}
*/
	//create HUD
	Super::ClientSetHUD_Implementation(NewHUDClass);
}

void ABaseSessionPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	SetShowMouseCursor(true);

	const FInputModeUIOnly InputMode = FInputModeUIOnly();
	SetInputMode(InputMode);
}

void ABaseSessionPlayerController::Multi_OnBattleStarted_Implementation(ABaseSessionPlayerState* StartingPlayer)
{
	//ToDo
	SetShowMouseCursor(false);
	const FInputModeGameOnly InputMode;
	SetInputMode(InputMode);
}

void ABaseSessionPlayerController::Multi_OnBattleEnded_Implementation(ABaseSessionPlayerState* EndingPlayer)
{
	//ToDo
}

void ABaseSessionPlayerController::Multi_OnBattleTimeOut_Implementation(ABaseSessionPlayerState* CurrentTurnPlayer)
{
	//ToDo
}
void ABaseSessionPlayerController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}

void ABaseSessionPlayerController::RegisterConsoleVariables()
{
	//ToDo
}