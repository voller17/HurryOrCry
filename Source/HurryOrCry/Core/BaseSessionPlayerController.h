// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseSessionPlayerState.h"
#include "GameFramework/PlayerController.h"
#include "BaseSessionPlayerController.generated.h"

UCLASS(Abstract)
class HURRYORCRY_API ABaseSessionPlayerController : public APlayerController
{
	GENERATED_BODY()
	
	ABaseSessionPlayerController();
	
public:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	virtual void InitPlayerState() override;

	virtual void PostInitializeComponents() override;
	
	virtual void ClientSetHUD_Implementation(TSubclassOf<AHUD> NewHUDClass) override;

	virtual void SetupInputComponent() override;

	//virtual void AddCheats(bool bForce = false) override;
	//virtual void EnableCheats() override; //ToDo

	virtual void Tick(float DeltaSeconds) override;

	//UFUNCTION(BlueprintCallable, BlueprintPure)
	//ABaseCamera* GetBaseCamera() const { return BaseCamera; };

	protected:
	virtual void BeginPlay() override;
	virtual void OnPossess(APawn* aPawn) override;

	UFUNCTION(NetMulticast, Reliable)
	virtual void Multi_OnBattleStarted(ABaseSessionPlayerState* StartingPlayer);
	virtual void Multi_OnBattleStarted_Implementation(ABaseSessionPlayerState* StartingPlayer);

	UFUNCTION(NetMulticast, Reliable)
	virtual void Multi_OnBattleEnded(ABaseSessionPlayerState* EndingPlayer);
	virtual void Multi_OnBattleEnded_Implementation(ABaseSessionPlayerState* EndingPlayer);
	
	UFUNCTION(NetMulticast, Reliable)
	virtual void Multi_OnBattleTimeOut(ABaseSessionPlayerState* CurrentTurnPlayer);
	virtual void Multi_OnBattleTimeOut_Implementation(ABaseSessionPlayerState* CurrentTurnPlayer);
	
	virtual void OnRep_PlayerState() override;

	virtual void SetPlayerState(ABaseSessionPlayerState* NewPlayerState);

	void RegisterConsoleVariables();
	

	//Variables
	
	UPROPERTY()
	ABaseSessionPlayerState* BasePlayerState = nullptr;
	
protected:

	UPROPERTY(BlueprintReadOnly)
	float FingerRadius = 50.f;
};
