// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "ToonTanksGameModeBase.generated.h"

class APawnTurret;
class APawnTank;
class APlayerControllerBase;

UCLASS()
class TOONTANKS_API AToonTanksGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

private:

	APawnTank* PlayerTank;
	int32 TargetTurrets = 0;
	APlayerControllerBase* PlayerControllerRef;

	int32 GetTargetTurretCount();
	void HandleGameStart();
	void HandleGameOver(bool PlayerWon);

public:

	void ActorDied(AActor* DeadActor);

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Game Loop")
		int32 StartDelay = 5;
	virtual void BeginPlay() override;
	UFUNCTION(BlueprintImplementableEvent)
		void GameStart();
	UFUNCTION(BlueprintImplementableEvent)
		void GameOver(bool PlayerWon);
};
