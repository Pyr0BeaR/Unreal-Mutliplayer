// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "OnlineSessionSettings.h"
#include "JoinGameCell.generated.h"

/**
 * 
 */
UCLASS()
class MULTIPLAYERSESSIONS_API UJoinGameCell : public UUserWidget
{
	GENERATED_BODY()
	
public:
	/*UFUNCTION(BlueprintCallable)*/
	void WidgetSetup(FOnlineSessionSearchResult result);

protected:
	UPROPERTY(BlueprintReadOnly, meta = (DisplayName = "Name"), meta = (ExposeOnSpawn = true))
	FString SessionName {TEXT("Game Name")};

	virtual bool Initialize() override;

	void OnJoinSession(EOnJoinSessionCompleteResult::Type Result);

private:
	UPROPERTY(meta = (BindWidget))
	class UButton* JoinButton;

	UFUNCTION()
	void JoinButtonClicked();

	FOnlineSessionSearchResult OnlineSessionSearchResult;

	class UMultiplayerSessionSubsystem* MultiplayerSessionSubsystem;
};
