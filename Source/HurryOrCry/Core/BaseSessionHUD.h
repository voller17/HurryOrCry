// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseSessionPlayerState.h"
#include "GameFramework/HUD.h"
#include "Widget/BattleUIWidget.h"
#include "BaseSessionHUD.generated.h"

UCLASS(Abstract)
class HURRYORCRY_API ABaseSessionHUD : public AHUD
{
	GENERATED_BODY()
	
public:
	virtual void BeginPlay () override;

	UFUNCTION()
	virtual void DisplayWaitingForPlayers();
	UFUNCTION()
	virtual void HideWaitingForPlayers();

	UFUNCTION()
	virtual void DisplayGameModeAnnouncement();
	UFUNCTION()
	virtual void HideGameModeAnnouncement();

	UFUNCTION()
	virtual void DisplayCharacterSelection();
	UFUNCTION()
	virtual void HideCharacterSelection();

	UFUNCTION()
	virtual void DisplayBattleUI();
	UFUNCTION()
	virtual void HideBattleUI();
	
	UFUNCTION()
	virtual void OnPlayerWin(ABaseSessionPlayerState* WinPlayer);

	//UFUNCTION()
	//void CompleteAsyncLoadBaseInterfaceWidget();//ToDo

	UFUNCTION()
	void BattleStarted(ABaseSessionPlayerState* PlayerState);

	//Variables
	protected:
	
	UPROPERTY(EditDefaultsOnly, Category="Settings")
	TSubclassOf<UUserWidget> WaitingForPlayersWidgetClass;
	
	UPROPERTY(EditDefaultsOnly, Category="Settings")
	TSubclassOf<UUserWidget> GameModeIntroWidgetClass;
	
	UPROPERTY(EditDefaultsOnly, Category="Settings")
	TSubclassOf<UUserWidget> ClassCharacterSelectionWidget;
	
	UPROPERTY(EditDefaultsOnly, Category="Settings")
	TSubclassOf<UUserWidget> ClassShowWinnerWidget;
	
	UPROPERTY(EditDefaultsOnly, Category="Settings")
	TSoftClassPtr<UBattleUIWidget> ClassBaseUIWidget;
	
	UPROPERTY()
	UUserWidget* WaitingForPlayersWidget;

	UPROPERTY(BlueprintReadWrite)
	UUserWidget* GameModeAnnouncementWidget;

	UPROPERTY()
	UUserWidget* CharacterSelectionWidget;
	
	UPROPERTY()
	UUserWidget* ShowWinnerWidget;

	UPROPERTY()
	UBattleUIWidget* BaseBattleUIWidget;
};