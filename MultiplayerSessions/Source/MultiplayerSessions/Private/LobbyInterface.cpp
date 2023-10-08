// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyInterface.h"
#include "Components/Button.h"
#include "Components/VerticalBox.h"
#include "GameFramework/GameModeBase.h"
#include "PlayerInformationCell.h"
#include "LobbyPlayerController.h"

void ULobbyInterface::MenuSetup()
{
	AddToViewport();
	SetVisibility(ESlateVisibility::Visible);

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
		}
	}

	if (StartButton)
	{
		StartButton->OnClicked.AddDynamic(this, &ThisClass::StartButtonClicked);
		if (World)
		{
			if (World->IsNetMode(ENetMode::NM_ListenServer))
			{
				StartButton->SetVisibility(ESlateVisibility::Visible);
				StartButton->SetIsEnabled(true);
			}
			
		}
	}
}

void ULobbyInterface::MenuTearDown()
{
	RemoveFromParent();
}

void ULobbyInterface::UpdatePlayerList(const TArray<FString>& Names, const TArray<int32>& IDs)
{
	PlayerListVerticalBox->ClearChildren();

	for (int32 i = 0; i < Names.Num(); i++)
	{
		if (PlayerInformationCellClass == nullptr) return;

		PlayerInformationCell = CreateWidget<UPlayerInformationCell>(this, PlayerInformationCellClass);
		PlayerInformationCell->WidgetSetup(Names[i], IDs[i]);
		PlayerListVerticalBox->AddChild(PlayerInformationCell);
	}

}

bool ULobbyInterface::Initialize()
{
	if (!Super::Initialize())
	{
		return false;
	}

	return true;
}

void ULobbyInterface::OnLevelRemovedFromWorld(ULevel* InLevel, UWorld* InWorld)
{
	//if (GEngine)
	//{
	//	GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Cyan, FString::Printf(TEXT("yes")));
	//}

	MenuTearDown();
	RemoveFromParent();

	UWorld* World = GetWorld();
	if (World)
	{
		if (PlayerController)
		{
			FInputModeGameOnly InputModeData;
			PlayerController->SetInputMode(InputModeData);
			PlayerController->SetShowMouseCursor(false);
		}
	}

	Super::OnLevelRemovedFromWorld(InLevel, InWorld);
}

void ULobbyInterface::StartButtonClicked()
{
	UWorld* World = GetWorld();
	if (World)
	{
		RemoveFromParent();
		AGameModeBase* GameMode = World->GetAuthGameMode<AGameModeBase>();
		if (GameMode)
		{
			GameMode->bUseSeamlessTravel = true;
			World->ServerTravel(FString(TEXT("/Game/Level/Level1?listen")));
		}
	}
}
