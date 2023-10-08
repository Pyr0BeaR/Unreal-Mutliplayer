// Fill out your copyright notice in the Description page of Project Settings.


#include "SearchGameMenu.h"
#include "JoinGameCell.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "Components/ScrollBox.h"
#include "Components/CircularThrobber.h"
#include "OnlineSessionSettings.h"
#include "MultiplayerSessionSubsystem.h"


void USearchGameMenu::MenuSetup()
{
	AddToViewport();
	SetVisibility(ESlateVisibility::Visible);
	bIsFocusable = true;

	UGameInstance* GameInstance = GetGameInstance();
	if (GameInstance)
	{
		MultiplayerSessionSubsystem = GameInstance->GetSubsystem<UMultiplayerSessionSubsystem>();
		if (MultiplayerSessionSubsystem)
		{
			MultiplayerSessionSubsystem->MultiplayerOnFindSessionComplete.AddUObject(this, &ThisClass::OnFindSession);
		}
	}

	if (RefreshButton)
	{
		RefreshButton->OnClicked.AddDynamic(this, &ThisClass::RefreshButtonClicked);
	}
}


bool USearchGameMenu::Initialize()
{
	if (!Super::Initialize())
	{
		return false;
	}

	return true;
}

void USearchGameMenu::OnLevelRemovedFromWorld(ULevel* InLevel, UWorld* InWorld)
{
	MenuTearDown();

	UWorld* World = GetWorld();
	if (World)
	{
		APlayerController* PlayerController = World->GetFirstPlayerController();
		if (PlayerController)
		{
			FInputModeGameOnly InputModeData;
			PlayerController->SetInputMode(InputModeData);
			PlayerController->SetShowMouseCursor(false);
		}
	}

	Super::OnLevelRemovedFromWorld(InLevel, InWorld);
}

void USearchGameMenu::OnFindSession(const TArray<FOnlineSessionSearchResult>& SessionResult, bool bWasSuccessful)
{
	if (MultiplayerSessionSubsystem == nullptr)
	{
		return;
	}

	for (auto result : SessionResult)
	{
		FString SettingValue;
		result.Session.SessionSettings.Get(FName("MatchType"), SettingValue);
		if (SettingValue == MatchType)
		{
			if (JoinGameCellClass)
			{
				JoinGameCell = CreateWidget<UJoinGameCell>(this, JoinGameCellClass);
				JoinGameCell->WidgetSetup(result);
				GameListScroll->AddChild(JoinGameCell);
			}
		}
	}

	RefreshButton->SetIsEnabled(true);

	CircularThrobber->SetVisibility(ESlateVisibility::Hidden);
}

void USearchGameMenu::RefreshButtonClicked()
{
	SearchGame();
}

void USearchGameMenu::SearchGame()
{
	RefreshButton->SetIsEnabled(false);

	CircularThrobber->SetVisibility(ESlateVisibility::Visible);

	GameListScroll->ClearChildren();

	if (MultiplayerSessionSubsystem)
	{
		MultiplayerSessionSubsystem->FindSession(10000);
	}
}

void USearchGameMenu::MenuTearDown()
{
	RemoveFromParent();
}