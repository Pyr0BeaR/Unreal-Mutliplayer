// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerInformationCell.generated.h"

/**
 * 
 */
class ALobbyPlayerController;
UCLASS()
class MULTIPLAYERSESSIONS_API UPlayerInformationCell : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void WidgetSetup(FString Name, int32 ID);

protected:

	UPROPERTY(BlueprintReadOnly)
	FString PlayerName {TEXT("Player Name")};

	int32 PlayerID;

	//virtual bool Initialize() override;

private:
	UPROPERTY(meta = (BindWidget))
	class UButton* RemoveButton;

	ALobbyPlayerController* LobbyPlayerController;

	UFUNCTION()
	void RemoveButtonClicked();
};
