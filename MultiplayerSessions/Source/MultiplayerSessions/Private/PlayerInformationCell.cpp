// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerInformationCell.h"
#include "Components/Button.h"
#include "GameFramework/PlayerState.h"
#include "LobbyPlayerController.h"
#include "Lobby_GameMode.h"

void UPlayerInformationCell::WidgetSetup(FString Name, int32 ID)
{
	PlayerName = Name;
	PlayerID = ID;		

	if (RemoveButton)
	{

		UWorld* World = GetWorld();
		if (World)
		{
			ALobby_GameMode* GameMode = Cast<ALobby_GameMode>(World->GetAuthGameMode<AGameModeBase>());
			if (GameMode)
			{
				LobbyPlayerController = LobbyPlayerController == nullptr ? GameMode->FindPCByID(PlayerID) : LobbyPlayerController;

				RemoveButton->OnClicked.AddDynamic(this, &ThisClass::RemoveButtonClicked);
				RemoveButton->SetVisibility(ESlateVisibility::Visible);
				RemoveButton->SetIsEnabled(true);
			}
		}
	}
}

void UPlayerInformationCell::RemoveButtonClicked()
{
	LobbyPlayerController->ClientLeaveGame();
}
