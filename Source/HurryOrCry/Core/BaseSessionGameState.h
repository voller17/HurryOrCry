#pragma once

#include "CoreMinimal.h"
#include "BaseSessionPlayerState.h"
#include "Defines.h"
#include "GameFramework/GameState.h"
#include "BaseSessionGameState.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FPlayerEventSignature, class ABaseSessionPlayerState*, Player);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FPlayerConnectedUpdateSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnWaitingForPlayersTimeOut);

UCLASS(Abstract)
class HURRYORCRY_API ABaseSessionGameState : public AGameState
{
	GENERATED_BODY()

	ABaseSessionGameState();
	
public:
	virtual void OnStartWaitingForPlayersTimer();

	virtual void AddPlayerState(APlayerState* PlayerState) override;

	virtual void RemovePlayerState(APlayerState* PlayerState) override;

	void SetGamePhase(EGamePhase NewPhase);
	
	void SetGameModeName(FString Name);
	void SetGameModeSlogan(FString Slogan);
	
	void SetTimeRemaining(float NewTime);

	virtual void OnWaitingForPlayersTimeOut();

	virtual void OnPlayersReady();
	
	UFUNCTION(NetMulticast, Reliable)
	void Multi_OnStartGameModeAnnouncementTimer();
	virtual void Multi_OnStartGameModeAnnouncementTimer_Implementation();

	UFUNCTION(BlueprintCallable)
	virtual TArray<ABaseSessionPlayerState*> GetPlayers() const;

	virtual bool IsWaitingForPlayers() const;

	virtual bool IsAnnouncingGameMode() const;

	UFUNCTION(BlueprintCallable, BlueprintPure)
	virtual FString GetGameModeName() const;

	UFUNCTION(BlueprintCallable, BlueprintPure)
	virtual FString GetGameModeSlogan() const;

protected:
	
	virtual void BeginPlay() override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	virtual void Auth_HandlePlayerVictory(ABaseSessionPlayerState* Player);

	virtual void OnCharacterSelection();
	
	virtual void OnStartPrepareBattle();
	
	virtual void OnStartBattle();

	virtual void SetWinnerPlayer(ABaseSessionPlayerState* NewWinnerPlayer);
	
	UFUNCTION()
	void OnRep_CurrentGamePhase();
	
	UFUNCTION()
	void OnRep_PlayerStates();
	
	//for client cosmetic timer
	UFUNCTION()
	void OnRep_TimeRemaining();
	
	//Returns winner player state if any
	virtual ABaseSessionPlayerState* GetWinnerPlayer() const;
	
	UFUNCTION(NetMulticast, Reliable)
	void Multi_AnnounceVictory(ABaseSessionPlayerState* Player);
	virtual void Multi_AnnounceVictory_Implementation(ABaseSessionPlayerState* Player);

	void OnNetworkFailure(UWorld* World, UNetDriver* NetDriver, ENetworkFailure::Type FailureType, const FString& ErrorMessage);

	//Variables
public:
	
	UPROPERTY(ReplicatedUsing = OnRep_CurrentGamePhase, BlueprintReadOnly, Category = "Game State")
	EGamePhase CurrentGamePhase;

	// Время до конца текущего раунда
	UPROPERTY(ReplicatedUsing = OnRep_TimeRemaining, BlueprintReadOnly, Category = "Game")
	float TimeRemaining;
	
	UPROPERTY(BlueprintReadWrite, Category="Settings|Cheats")
	bool bAllowCheatManager = true;

	UPROPERTY(BlueprintAssignable)
	FSimpleEventSignature OnStartWaitingForPlayersDelegate;

	UPROPERTY(BlueprintAssignable)
	FSimpleEventSignature OnFinishWaitingForPlayersDelegate;

	UPROPERTY(BlueprintAssignable)
	FSimpleEventSignature OnStartPrepareBattleDelegate;
	
	UPROPERTY(BlueprintAssignable)
	FSimpleEventSignature OnStartBattleDelegate;

	UPROPERTY(BlueprintAssignable)
	FSimpleEventSignature OnStartGameModeAnnouncementDelegate;
	UPROPERTY(BlueprintAssignable)
	FSimpleEventSignature OnFinishGameModeAnnouncementDelegate;

	UPROPERTY(BlueprintAssignable)
	FSimpleEventSignature OnStartCharacterSelectionDelegate;
	UPROPERTY(BlueprintAssignable)
	FSimpleEventSignature OnFinishCharacterSelectionDelegate;

	UPROPERTY(BlueprintAssignable)
	FPlayerConnectedUpdateSignature OnPlayerConnectedUpdate;
	
	UPROPERTY(BlueprintAssignable)
	FPlayerEventSignature OnPlayerWinDelegate;

	UPROPERTY(BlueprintAssignable)
	FOnWaitingForPlayersTimeOut OnWaitingForPlayersTimeOutDelegate;
	
	UPROPERTY(BlueprintAssignable)
	FPlayerEventSignature OnTurnTimeOutDelegate;
	
protected:

	UPROPERTY(ReplicatedUsing = OnRep_PlayerStates)
	TArray<ABaseSessionPlayerState*> PlayerStates;

	int8 CurrentTurnPlayerIndex = -1;
	
	UPROPERTY()
	ABaseSessionPlayerState* WinnerPlayer;

	//Name of a game mode players will see
	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Game")
	FString GameModeName = TEXT("Test");
	
	//Slogan of a game mode players will see
	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Game")
	FString GameModeSlogan = TEXT("TestSlogan");

};