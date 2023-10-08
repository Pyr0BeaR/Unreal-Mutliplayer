// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Interfaces/OnlineSessionInterface.h"

#include "SearchGameMenu.generated.h"

/**
 * 
 */
UCLASS()
class MULTIPLAYERSESSIONS_API USearchGameMenu : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void MenuSetup();

	UFUNCTION(BlueprintCallable)
	void SearchGame();

	UFUNCTION(BlueprintCallable)
	void MenuTearDown();

protected:
	virtual bool Initialize() override;
	virtual void OnLevelRemovedFromWorld(ULevel* InLevel, UWorld* InWorld) override;

	void OnFindSession(const TArray<FOnlineSessionSearchResult>& SessionResult, bool bWasSuccessful);

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<class UJoinGameCell> JoinGameCellClass;

	UPROPERTY()
	class UJoinGameCell* JoinGameCell;

private:

	UPROPERTY(meta = (BindWidget))
	class UButton* RefreshButton;

	UPROPERTY(meta = (BindWidget))
	class UScrollBox* GameListScroll;

	UPROPERTY(meta = (BindWidget))
	class UCircularThrobber* CircularThrobber;



	UPROPERTY()
	class UMultiplayerSessionSubsystem* MultiplayerSessionSubsystem;

	UFUNCTION()
	void RefreshButtonClicked();

	FString MatchType{ TEXT("FreeForAll") };
};
