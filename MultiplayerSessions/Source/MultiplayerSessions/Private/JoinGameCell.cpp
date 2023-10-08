// Fill out your copyright notice in the Description page of Project Settings.


#include "JoinGameCell.h"
#include "Components/Button.h"
#include "MultiplayerSessionSubsystem.h"
#include "OnlineSessionSettings.h"
#include "OnlineSubsystem.h"

void UJoinGameCell::WidgetSetup(FOnlineSessionSearchResult result)
{
	OnlineSessionSearchResult = result;
	SessionName = FString::Printf(TEXT("%s's Game"), *(OnlineSessionSearchResult.Session.OwningUserName));
}

bool UJoinGameCell::Initialize()
{
	if (!Super::Initialize())
	{
		return false;
	}

	UGameInstance* GameInstance = GetGameInstance();
	if (GameInstance)
	{
		MultiplayerSessionSubsystem = GameInstance->GetSubsystem<UMultiplayerSessionSubsystem>();
	}

	if (MultiplayerSessionSubsystem)
	{
		MultiplayerSessionSubsystem->MultiplayerOnJoinSessionComplete.AddUObject(this, &ThisClass::OnJoinSession);
	}

	if (JoinButton)
	{
		JoinButton->OnClicked.AddDynamic(this, &ThisClass::JoinButtonClicked);
	}

	
	return true;
}

void UJoinGameCell::OnJoinSession(EOnJoinSessionCompleteResult::Type Result)
{
	IOnlineSubsystem* Subsystem = IOnlineSubsystem::Get();
	if (Subsystem)
	{
		IOnlineSessionPtr SessionInterface = Subsystem->GetSessionInterface();
		if (SessionInterface.IsValid())
		{
			FString Address;
			SessionInterface->GetResolvedConnectString(NAME_GameSession, Address);

			APlayerController* PlayerController = GetGameInstance()->GetFirstLocalPlayerController();
			if (PlayerController)
			{
				PlayerController->ClientTravel(Address, ETravelType::TRAVEL_Absolute);
			}
		}
	}
}


void UJoinGameCell::JoinButtonClicked()
{
	
	JoinButton->SetIsEnabled(false);
	if (MultiplayerSessionSubsystem)
	{
		if (OnlineSessionSearchResult.IsValid())
		{
			MultiplayerSessionSubsystem->JoinSession(OnlineSessionSearchResult);
		}
	}
}
