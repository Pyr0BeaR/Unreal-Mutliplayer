// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "LobbyPlayerController.generated.h"
/**
 * 
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnLeftGame);

UCLASS()
class MULTIPLAYERSESSIONS_API ALobbyPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void LobbyWidgetSetup();

	UFUNCTION(Server, Reliable, BlueprintCallable)
	void UpdatePlayer();

	UFUNCTION(Client, Reliable)
	void ClientUpdateWidget(const TArray<FString>& Names, const TArray<int32>& IDs);

	UFUNCTION(Client, Reliable)
	void ClientLeaveGame();

	FOnLeftGame OnLeftGame;

protected:
	virtual void SetupInputComponent() override;
	virtual void BeginPlay() override;

	virtual void PostInitializeComponents() override;

	void ShowOptionMenu();

	/*
	*Option Menu
	*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = HUD)
	TSubclassOf<class UOptionMenu> OptionMenuClass;

	UPROPERTY()
	UOptionMenu* OptionMenu;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = HUD)
	TSubclassOf<class ULobbyInterface> LobbyInterfaceClass;

	UPROPERTY()
	ULobbyInterface* LobbyInterface;

	bool bOptionMenuOpen = false;

	bool bLeftGame = false;

	UFUNCTION()
	void OnDestroySession(bool bWasSuccessful);

private:
	TArray<FString> NameList;
	TArray<int32> IDList;

	class UMultiplayerSessionSubsystem* MultiplayerSessionSubsystem;

};
