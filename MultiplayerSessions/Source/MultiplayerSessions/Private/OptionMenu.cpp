// Fill out your copyright notice in the Description page of Project Settings.


#include "OptionMenu.h"
#include "Components/Button.h"
#include "GameFramework/GameModeBase.h"
#include "GameFramework/Controller.h"
#include "MultiplayerSessionSubsystem.h"
#include "LobbyPlayerController.h"

void UOptionMenu::MenuSetup()
{
	AddToViewport();
	SetVisibility(ESlateVisibility::Visible);
	bIsFocusable = true;

	UWorld* World = GetWorld();
	if (World)
	{
		PlayerController = PlayerController == nullptr ? World->GetFirstPlayerController() : PlayerController;
		if (PlayerController)
		{
			FInputModeGameAndUI InputModeData;
			InputModeData.SetWidgetToFocus(TakeWidget());
			PlayerController->SetInputMode(InputModeData);
			PlayerController->SetShowMouseCursor(true);
			PlayerController->SetIgnoreMoveInput(true);
		}
	}

	if (LeaveButton && !LeaveButton->OnClicked.IsBound())
	{
		LeaveButton->OnClicked.AddDynamic(this, &ThisClass::LeaveButtonClicked);
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

void UOptionMenu::MenuTearDown()
{
	RemoveFromParent();
	UWorld* World = GetWorld();
	if (World)
	{
		PlayerController = PlayerController == nullptr ? World->GetFirstPlayerController() : PlayerController;
		if (PlayerController)
		{
			FInputModeGameAndUI InputModeData;
			PlayerController->SetInputMode(InputModeData);
			PlayerController->SetIgnoreMoveInput(false);
		}
	}

	if (LeaveButton && LeaveButton->OnClicked.IsBound())
	{
		LeaveButton->OnClicked.RemoveDynamic(this, &ThisClass::LeaveButtonClicked);
	}

	if (MultiplayerSessionSubsystem && MultiplayerSessionSubsystem->MultiplayerOnDestroySessionComplete.IsBound())
	{
		MultiplayerSessionSubsystem->MultiplayerOnDestroySessionComplete.RemoveDynamic(this, &ThisClass::OnDestroySession);
	}

}

bool UOptionMenu::Initialize()
{
	if (!Super::Initialize())
	{
		return false;
	}

	return true;
}

void UOptionMenu::OnDestroySession(bool bWasSuccessful)
{
	if (!bWasSuccessful)
	{
		LeaveButton->SetIsEnabled(true);
		if (PlayerController)
		{
			PlayerController->SetShowMouseCursor(true);
		}
		return;
	}

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
			PlayerController = PlayerController == nullptr ? World->GetFirstPlayerController() : PlayerController;
			if (PlayerController)
			{
				PlayerController->ClientReturnToMainMenuWithTextReason(FText());
			}
		}
	}
}

void UOptionMenu::LeaveButtonClicked()
{
	
	if (PlayerController)
	{
		PlayerController->SetShowMouseCursor(false);
	}

	LeaveButton->SetIsEnabled(false);


	UWorld* World = GetWorld();
	if (World)
	{
		ALobbyPlayerController* FirstPlayerController = Cast<ALobbyPlayerController>(World->GetFirstPlayerController());
		if (FirstPlayerController)
		{
			FirstPlayerController->OnLeftGame.AddDynamic(this, &ThisClass::OnPlayerLeftGame);
			//FirstPlayerController->ServerLeaveGame();
			OnPlayerLeftGame();
			
		}
		else
		{
			LeaveButton->SetIsEnabled(true);
		}
		OnPlayerLeftGame();
	}
}

void UOptionMenu::OnPlayerLeftGame()
{
	if (MultiplayerSessionSubsystem)
	{
		MultiplayerSessionSubsystem->DestroySession();
	}
}