// Fill out your copyright notice in the Description page of Project Settings.


#include "Lobby_GameMode.h"
#include "LobbyPlayerController.h"
#include "GameFramework/GameStateBase.h"
#include "GameFramework/PlayerState.h"

void ALobby_GameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	ALobbyPlayerController* LobbyPlayerController = Cast<ALobbyPlayerController>(NewPlayer);
	PC_List.AddUnique(LobbyPlayerController);
	//LobbyPlayerController->UpdatePlayer();
}

void ALobby_GameMode::Logout(AController* Exiting)
{
	Super::Logout(Exiting);

	ALobbyPlayerController* LobbyPlayerController = Cast<ALobbyPlayerController>(Exiting);
	PC_List.Remove(LobbyPlayerController);
	LobbyPlayerController->UpdatePlayer();
	
}

ALobbyPlayerController* ALobby_GameMode::FindPCByID(int32 ID)
{
	for (auto pc : PC_List)
	{
		APlayerState* PS = pc->GetPlayerState<APlayerState>();
		if (PS)
		{
			if (ID == PS->GetPlayerId())
			{
				return pc;
			}
		}
	}

	return nullptr;
}
