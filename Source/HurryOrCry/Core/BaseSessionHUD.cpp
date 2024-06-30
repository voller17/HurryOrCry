// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseSessionHUD.h"

#include "BaseSessionGameState.h"

void ABaseSessionHUD::BeginPlay()
{
	Super::BeginPlay();

	if (ABaseSessionGameState* GameState = GetWorld()->GetGameState<ABaseSessionGameState>())
	{
		if (GameState->IsWaitingForPlayers())
		{
			DisplayWaitingForPlayers();
			GameState->OnFinishWaitingForPlayersDelegate.AddDynamic(this, &ABaseSessionHUD::HideWaitingForPlayers);
		}
		if (GameState->IsAnnouncingGameMode())
		{
			DisplayGameModeAnnouncement();
		}
		else
		{
			GameState->OnStartGameModeAnnouncementDelegate.AddDynamic(this, &ABaseSessionHUD::DisplayGameModeAnnouncement);
		}
		GameState->OnFinishGameModeAnnouncementDelegate.AddDynamic(this, &ABaseSessionHUD::HideGameModeAnnouncement);
		
		GameState->OnStartCharacterSelectionDelegate.AddDynamic(this, &ABaseSessionHUD::DisplayCharacterSelection);		
		GameState->OnFinishCharacterSelectionDelegate.AddDynamic(this, &ABaseSessionHUD::HideCharacterSelection);
		
		GameState->OnStartPrepareBattleDelegate.AddDynamic(this, &ABaseSessionHUD::DisplayBattleUI);
		
		GameState->OnPlayerWinDelegate.AddDynamic(this, &ABaseSessionHUD::OnPlayerWin);
	}

}

void ABaseSessionHUD::DisplayWaitingForPlayers()
{
	if (WaitingForPlayersWidgetClass.Get())
	{
		WaitingForPlayersWidget = CreateWidget(GetOwningPlayerController(), WaitingForPlayersWidgetClass);
		if (WaitingForPlayersWidget)
		{
			WaitingForPlayersWidget->AddToViewport(1);
		}
	}
}

void ABaseSessionHUD::HideWaitingForPlayers()
{
	if (WaitingForPlayersWidget)
	{
		WaitingForPlayersWidget->RemoveFromParent();
	}
}

void ABaseSessionHUD::DisplayGameModeAnnouncement()
{
	
	if (GameModeIntroWidgetClass.Get())
	{
		GameModeAnnouncementWidget = CreateWidget(GetOwningPlayerController(), GameModeIntroWidgetClass);
		if (GameModeAnnouncementWidget)
		{
			GameModeAnnouncementWidget->AddToViewport(1);
		}
	}
}

void ABaseSessionHUD::HideGameModeAnnouncement()
{
	if (GameModeAnnouncementWidget)
	{
		GameModeAnnouncementWidget->RemoveFromParent();
	}
	//For now lets just use LoadSynchronous() because asnyc loading is not working in editor on first start for some reason
	/*FStreamableManager AssetLoader;
	const FSoftObjectPath AssetToLoad = ClassBaseInterfaceWidget.ToSoftObjectPath();
	AssetLoader.RequestAsyncLoad(AssetToLoad, FStreamableDelegate::CreateUObject(this, &ABaseHUD::CompleteAsyncLoadBaseInterfaceWidget));
	*/
}

void ABaseSessionHUD::DisplayCharacterSelection()
{
	if (ClassCharacterSelectionWidget.Get())
	{
		CharacterSelectionWidget = CreateWidget(GetOwningPlayerController(), ClassCharacterSelectionWidget);
		if (CharacterSelectionWidget)
		{
			CharacterSelectionWidget->AddToViewport(1);
		}
	}
}

void ABaseSessionHUD::HideCharacterSelection()
{
	if (CharacterSelectionWidget)
	{
		CharacterSelectionWidget->RemoveFromParent();
	}
	DisplayBattleUI();
}

void ABaseSessionHUD::DisplayBattleUI()
{
	BaseBattleUIWidget = Cast<UBattleUIWidget>(CreateWidget(GetWorld(), ClassBaseUIWidget.LoadSynchronous()));
	if (BaseBattleUIWidget)
	{
		BaseBattleUIWidget->AddToViewport();
	}
}

void ABaseSessionHUD::HideBattleUI()
{
	if (BaseBattleUIWidget)
	{
		BaseBattleUIWidget->RemoveFromParent();
	}
}


void ABaseSessionHUD::OnPlayerWin(ABaseSessionPlayerState* WinPlayer)
{
	if (WinPlayer)
	{
		//display normal victory widget ToDo
	}
}

void ABaseSessionHUD::BattleStarted(ABaseSessionPlayerState* PlayerState)
{

	if (BaseBattleUIWidget)
	{
		BaseBattleUIWidget->AnnounceBattle(PlayerState);
	}
}
/*
void ABaseSessionHUD::CompleteAsyncLoadBaseInterfaceWidget()
{
	//BaseBattleUIWidget = Cast<UBattleUIWidget>(CreateWidget(GetWorld(), ClassBaseInterfaceWidget.Get()));
	//if (BaseBattleUIWidget)
	{
		//BaseBattleUIWidget->AddToViewport();
		//BaseBattleUIWidget->PlayerTurn();
	};
}
*///ToDo
