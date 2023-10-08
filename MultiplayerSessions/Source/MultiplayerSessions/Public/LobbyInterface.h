// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LobbyInterface.generated.h"

/**
 * 
 */
UCLASS()
class MULTIPLAYERSESSIONS_API ULobbyInterface : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable)
	void MenuSetup();

	UFUNCTION(BlueprintCallable)
	void MenuTearDown();

	UFUNCTION(BlueprintCallable)
	void UpdatePlayerList(const TArray<FString>& Names, const TArray<int32>& IDs);

protected:
	virtual bool Initialize() override;

	virtual void OnLevelRemovedFromWorld(ULevel* InLevel, UWorld* InWorld) override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<class UPlayerInformationCell> PlayerInformationCellClass;

	UPROPERTY()
	UPlayerInformationCell* PlayerInformationCell;

private:
	class APlayerController* PlayerController;

	UPROPERTY(meta = (BindWidget))
	class UButton* StartButton;

	UPROPERTY(meta = (BindWidget))
	class UVerticalBox* PlayerListVerticalBox;



	UFUNCTION()
	void StartButtonClicked();
};
