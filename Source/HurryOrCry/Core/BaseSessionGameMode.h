// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Defines.h"
#include "GameFramework/GameMode.h"
#include "BaseSessionGameMode.generated.h"

/**
 * 
 */
UCLASS(Abstract)
class HURRYORCRY_API ABaseSessionGameMode : public AGameMode
{
	GENERATED_BODY()

	ABaseSessionGameMode();
	
protected:
	virtual void BeginPlay() override;	
	virtual FString InitNewPlayer(APlayerController* NewPlayerController, const FUniqueNetIdRepl& UniqueId, const FString& Options, const FString& Portal) override;
	virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;
	//GamePhase----
	virtual void OnWaitingForPlayersTimeOut();
	
	virtual void OnPlayersReady();
	
	virtual void StartCharacterSelection();
	virtual void FinishCharacterSelection();
	
	virtual void StartPrepareBattle();
	virtual void FinishPrepareBattle();
	
	virtual void StartBattle();
	virtual void FinishBattle();
	
	virtual void StartGameModeAnnouncementTimer();
	virtual void OnGameModeAnnouncementTimeOut();
	//GamePhase----
public:
		
	void SetGamePhase(EGamePhase NewPhase);
	
	virtual bool IsWaitingForPlayers() const;
	virtual void StartWaitingForPlayersTimer();
	
	virtual TArray<APlayerController*> GetPlayingControllers();
	
	virtual void StartPlay() override;
	virtual void EndMatch() override;	
	
	virtual void HandleMatchAborted() override;
	virtual bool ReadyToEndMatch_Implementation() override;
	virtual bool ReadyToStartMatch_Implementation() override;
	virtual void Logout(AController* Exiting) override;
	
	UFUNCTION(BlueprintCallable, BlueprintPure)
	bool HasGameStarted() const;

private:
	void ServerTerminationCheck();
	void ServerRequestExit();

	//Variables
protected:
	UPROPERTY()
	TArray<APlayerController*> PlayingControllers;

	UPROPERTY(EditAnywhere, Category="Settings|Gameplay")
	int MaxPlayers = 2;

	bool bHasGameInitiated = false;
	bool bHasGameStarted = false;

	//How long should wait players connected
	UPROPERTY(EditAnywhere, Category="Settings|Gameplay")
	float WaitingForPlayersTimeLimit = 30.f;

	//How long should a game mode announcement message stay on screen for
	UPROPERTY(EditAnywhere, Category="Settings|Gameplay")
	float GameModeAnnouncementDuration = 2.f;

	//How long player can selected character
	UPROPERTY(EditAnywhere, Category="Settings|Gameplay")
	float CharacterSelectedDuration = 40.f;

	//How long player can buy items
	UPROPERTY(EditAnywhere, Category="Settings|Gameplay")
	float PrepareBattleDuration = 30.f;

	//How long battle
	UPROPERTY(EditAnywhere, Category="Settings|Gameplay")
	float BattleDuration = 60.f;
	
	UPROPERTY(EditAnywhere, Category="Settings|Gameplay")
	float DelayBeforeStart = 1.0f;
	
	//Time delay between when the match is concluded and score screen is shown
	float ScoreScreenDelay = 2.f;
		
	//Name of a game mode players will see
	UPROPERTY(EditAnywhere, Category="Settings|Gameplay")
	FString GameModeName = TEXT("Battle Arena");
	
	//Slogan of a game mode players will see
	UPROPERTY(EditAnywhere, Category="Settings|Gameplay")
	FString GameModeSlogan = TEXT("«There are no shortcuts to any place worth going»");
	
	//UPROPERTY()
	//TArray<AUnitStartPoint*> UnitsStartPoints;//ToDo
	
private:

	FTimerHandle WaitingForPlayersTimerHandle;
	FTimerHandle GameModeAnnouncementTimerHandle;
	FTimerHandle CharacterSelectionTimerHandle;
	FTimerHandle PrepareBattleTimerHandle;
	FTimerHandle BattleTimerHandle;
	
	FTimerHandle ServerTerminateTimerHandle;
	
	UPROPERTY()
	EGamePhase CurrentGamePhase;
};
