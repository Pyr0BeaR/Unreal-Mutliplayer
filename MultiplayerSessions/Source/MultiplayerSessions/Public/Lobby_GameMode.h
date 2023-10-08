// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "Lobby_GameMode.generated.h"

class ALobbyPlayerController;
/**
 * 
 */
UCLASS()
class MULTIPLAYERSESSIONS_API ALobby_GameMode : public AGameMode
{
	GENERATED_BODY()
	
public:
	virtual void PostLogin(APlayerController* NewPlayer) override;
	virtual void Logout(AController* Exiting) override;

	ALobbyPlayerController* FindPCByID(int32 ID);

	TArray<TObjectPtr<ALobbyPlayerController>> PC_List;
	//TArray<ALobbyPlayerController*> PC_List;

};
