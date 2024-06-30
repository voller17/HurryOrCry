#pragma once

#include "CoreMinimal.h"
#include "BaseCharacter.h"
#include "GameFramework/PlayerState.h"
#include "BaseSessionPlayerState.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FPlayerStateEventSignature, class ABaseSessionPlayerState*, PlayerState);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FPlayerStateUnitEventSignature, class ABaseSessionPlayerState*, PlayerState, ABaseCharacter*, BaseCharacter);


UCLASS(Abstract)
class HURRYORCRY_API ABaseSessionPlayerState : public APlayerState
{
	GENERATED_BODY()
	
public:
	ABaseSessionPlayerState();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void ClientInitialize(AController* C) override;
	UFUNCTION(NetMulticast, Reliable)
	virtual void Multi_OnStartBattle();
	virtual void Multi_OnStartBattle_Implementation();
	UFUNCTION()
	virtual void OnEndBattle();
	
	virtual void OnRep_PlayerName() override;
	//Used to set player index by server based on PlayerStates array in BaseGameState
	void SetPlayerIndex(int32 Index);
	virtual void Tick(float DeltaSeconds) override;
	
	//Returns index that is determined by an order in which a player joined a match
	UFUNCTION(BlueprintCallable, BlueprintPure)
	int32 GetPlayerIndex() const;
	
	UFUNCTION(BlueprintCallable, BlueprintPure)
	virtual TSubclassOf<ABaseCharacter> GetSelectedCharacter() const;

	UFUNCTION(BlueprintCallable, BlueprintPure)
	bool IsCharacterSelected() const;
	
	UFUNCTION(BlueprintCallable, Category = "Character Selection")
	void ClientSelectCharacter(const FString& NewSelectedCharacter);
	
	UFUNCTION(Client,Reliable)
	void ClientGetCustomPlayerName();
	void ClientGetCustomPlayerName_Implementation();
//----------------------------------------------------------
	
protected:
	
	virtual void BeginPlay() override;
	virtual void PostInitializeComponents() override;
	//void SendDeckUnitsClasses();
	
	UFUNCTION(Server, Reliable, WithValidation)
	void ServerSelectCharacter(const FString& NewSelectedCharacter);
	void ServerSelectCharacter_Implementation(const FString& NewSelectedCharacter);
	bool ServerSelectCharacter_Validate(const FString& NewSelectedCharacter);

	UFUNCTION(Server, Reliable)
	void ServerSetCustomPlayerName(const FString& NewName);
	void ServerSetCustomPlayerName_Implementation(const FString& NewName);
	
	//UFUNCTION(Server, Reliable)
	//void Server_Replicate();
	//void Server_Replicate_Implementation(); ToDo
	
	//Variables

public:
	
	UPROPERTY(BlueprintAssignable)
	FPlayerStateEventSignature OnBattleStartedDelegate;

	UPROPERTY(BlueprintAssignable)
	FPlayerStateEventSignature OnNameChangedDelegate;

	UPROPERTY(BlueprintAssignable)
	FPlayerStateEventSignature OnBattleEndedDelegate;

	UPROPERTY(BlueprintAssignable)
	FPlayerStateEventSignature OnBattleTimeOutDelegate;

	UPROPERTY(BlueprintAssignable)
	FPlayerStateUnitEventSignature OnUnitSetDelegate;

protected:
	//Indexed in order of joining a match
	UPROPERTY(Replicated)
	int32 PlayerIndex = 0;

	UPROPERTY(Replicated)
	bool bIsCharacterSelected;
	
	UPROPERTY(Replicated)
	TSubclassOf<ABaseCharacter> SelectedCharacter = ABaseCharacter::StaticClass();
};
