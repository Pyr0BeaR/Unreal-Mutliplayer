// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "OptionMenu.generated.h"

/**
 * 
 */
UCLASS()
class MULTIPLAYERSESSIONS_API UOptionMenu : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable)
	void MenuSetup();

	UFUNCTION(BlueprintCallable)
	void MenuTearDown();

protected:
	virtual bool Initialize() override;

	UFUNCTION()
	void OnDestroySession(bool bWasSuccessful);

	UFUNCTION()
	void OnPlayerLeftGame();

private:
	class UMultiplayerSessionSubsystem* MultiplayerSessionSubsystem;

	class APlayerController* PlayerController;

	UPROPERTY(meta = (BindWidget))
	class UButton* LeaveButton;

	UFUNCTION()
	void LeaveButtonClicked();
};
