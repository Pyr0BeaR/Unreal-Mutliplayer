// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyPlayerController.h"
#include "OptionMenu.h"
#include "LobbyInterface.h"
#include "Blueprint/UserWidget.h"
#include "Lobby_GameMode.h"
#include "Kismet/KismetSystemLibrary.h"
#include "GameFramework/Actor.h"
#include "GameFramework/PlayerState.h"
#include "MultiplayerSessionSubsystem.h"

void ALobbyPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	if (InputComponent == nullptr) return;

	InputComponent->BindAction("Option", IE_Pressed, this, &ThisClass::ShowOptionMenu);
}

void ALobbyPlayerController::BeginPlay()
{
	Super::BeginPlay();
}

void ALobbyPlayerController::PostInitializeComponents()
{
	Super::PostInitializeComponents();
}

void ALobbyPlayerController::ShowOptionMenu()
{
	if (OptionMenuClass == nullptr) return;
	if (OptionMenu == nullptr)
	{
		OptionMenu = CreateWidget<UOptionMenu>(this, OptionMenuClass);
	}
	if (OptionMenu)
	{
		bOptionMenuOpen = !bOptionMenuOpen;
		if (bOptionMenuOpen)
		{
			OptionMenu->MenuSetup();
		}
		else
		{
			OptionMenu->MenuTearDown();
		}
	}
}

void ALobbyPlayerController::LobbyWidgetSetup()
{
	if(IsLocalController())
	{ 
		if (LobbyInterfaceClass == nullptr) return;
		if (LobbyInterface == nullptr)
		{
			LobbyInterface = CreateWidget<ULobbyInterface>(this, LobbyInterfaceClass);
			LobbyInterface->MenuSetup();
		}
	}

	UGameInstance* GameInstance = GetGameInstance();
	if (GameInstance)
	{
		MultiplayerSessionSubsystem = GameInstance->GetSubsystem<UMultiplayerSessionSubsystem>();
		if (MultiplayerSessionSubsystem)
		{
			MultiplayerSessionSubsystem->MultiplayerOnDestroySessionComplete.AddDynamic(this, &ThisClass::OnDestroySession);
		}
	}
}

void ALobbyPlayerController::UpdatePlayer_Implementation()
{
	UWorld* World = GetWorld();
	if (World)
	{
		ALobby_GameMode* GameMode = Cast<ALobby_GameMode>(World->GetAuthGameMode<AGameModeBase>());
		if (GameMode)
		{
			TArray<FString> Names;
			TArray<int32> IDs;

			for (auto pc : GameMode->PC_List)
			{
				APlayerState* PS = pc->GetPlayerState<APlayerState>();
				if (PS)
				{
					FString name = PS->GetPlayerName();
					Names.Add(name);
					int32 ID = PS->GetPlayerId();
					IDs.Add(ID);
					/*if (GEngine)
					{
						GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Cyan, FString::Printf(TEXT("%s, %i"), *name,ID));
					}*/
				}
			}

			for (auto pc : GameMode->PC_List)
			{
				pc->ClientUpdateWidget(Names, IDs);
			}
		}
	}
}

void ALobbyPlayerController::ClientUpdateWidget_Implementation(const TArray<FString>& Names, const TArray<int32>& IDs)
{
	if (LobbyInterface != nullptr)
	{
		LobbyInterface->UpdatePlayerList(Names, IDs);
	}
}

void ALobbyPlayerController::OnDestroySession(bool bWasSuccessful)
{
	UWorld* World = GetWorld();
	if (World)
	{
		AGameModeBase* GameMode = World->GetAuthGameMode<AGameModeBase>();
		if (GameMode)
		{
			GameMode->ReturnToMainMenuHost();
		}
		else
		{
			ClientReturnToMainMenuWithTextReason(FText());
		}
	}
}

void ALobbyPlayerController::ClientLeaveGame_Implementation()
{
	if (MultiplayerSessionSubsystem)
	{
		MultiplayerSessionSubsystem->DestroySession();
	}

	//OnLeftGame.Broadcast();
}