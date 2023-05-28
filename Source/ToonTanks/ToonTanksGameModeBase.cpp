// Fill out your copyright notice in the Description page of Project Settings.

#include "ToonTanksGameModeBase.h"
#include "Public/PawnTank.h"
#include "Public/PawnTurret.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "TimerManager.h"
#include "Public/PlayerControllerBase.h"

void AToonTanksGameModeBase::BeginPlay()
{
    Super::BeginPlay();

    HandleGameStart();
}
// Check what type of actor died. If turret destroy, if player go to lose condition
void AToonTanksGameModeBase::ActorDied(AActor* DeadActor)
{
    // UE_LOG(LogTemp, Warning, TEXT("A Pawn Died"));
    if (DeadActor == PlayerTank)
    {
        PlayerTank->HandleDestruction();
        HandleGameOver(false);

        if (PlayerControllerRef)
        {
            PlayerControllerRef->SetPlayerEnabledState(false);
        }
    }
    else if (APawnTurret* DestroyedTurret = Cast<APawnTurret>(DeadActor))
    {
        DestroyedTurret->HandleDestruction();
        if (--TargetTurrets <= 0)
            HandleGameOver(true);
    }
}
// Initial the start countdown, turret activation, pawn check, etc.
void AToonTanksGameModeBase::HandleGameStart()
{
    TargetTurrets = GetTargetTurretCount();
    // UE_LOG(LogTemp, Warning, TEXT("Targets:%i"),TargetTurrets);
    PlayerTank = Cast<APawnTank>(UGameplayStatics::GetPlayerPawn(this, 0));
    PlayerControllerRef = Cast<APlayerControllerBase>(UGameplayStatics::GetPlayerController(this, 0));

    GameStart();
    if (PlayerControllerRef)
    {
        PlayerControllerRef->SetPlayerEnabledState(false);

        FTimerHandle PlayerEnableHandle;
        FTimerDelegate PlayerEnableDelegate = FTimerDelegate::CreateUObject(PlayerControllerRef, &APlayerControllerBase::SetPlayerEnabledState, true);
        GetWorld()->GetTimerManager().SetTimer(PlayerEnableHandle, PlayerEnableDelegate, StartDelay, false);
    }
}
// Check if all turret destroy show win result, else show lose and call blueprint version
void AToonTanksGameModeBase::HandleGameOver(bool PlayerWon)
{
    GameOver(PlayerWon);
}
int32 AToonTanksGameModeBase::GetTargetTurretCount()
{
    TArray<AActor*> TurretActors;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), APawnTurret::StaticClass(), TurretActors);
    return TurretActors.Num();
}
