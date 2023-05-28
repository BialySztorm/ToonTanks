// Fill out your copyright notice in the Description page of Project Settings.


#include "PawnTurret.h"
#include "Kismet/GameplayStatics.h"
#include "PawnTank.h"
#include "Engine/World.h"
#include "TimerManager.h"

void APawnTurret::BeginPlay()
{
	Super::BeginPlay();

	GetWorld()->GetTimerManager().SetTimer(FireRateTimerHandle, this, &APawnTurret::ChechFireCondition, FireRate, true);

	PlayerPawn = Cast<APawnTank>(UGameplayStatics::GetPlayerPawn(this, 0));


}

void APawnTurret::HandleDestruction()
{
	Super::HandleDestruction();

	Destroy();
}

void APawnTurret::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!PlayerPawn || ReturnDistanceToPlayer() > FireRange)
		return;
	// UE_LOG(LogTemp, Warning,TEXT("RotationCondition Succesed!"));
	RotateTurret(PlayerPawn->GetActorLocation());
}
void APawnTurret::ChechFireCondition()
{
	// If player == null || is dead then FireRateTimerHandle
	if (!PlayerPawn || !PlayerPawn->GetIsPlayerAlive())
	{
		return;
	}
	// If player is in range then fire
	if (ReturnDistanceToPlayer() <= FireRange)
	{
		Fire();
		// UE_LOG(LogTemp, Warning,TEXT("FireCondition Succesed!"));
	}

}

float APawnTurret::ReturnDistanceToPlayer()
{
	if (!PlayerPawn)
		return 0.0f;

	return FVector::Dist(PlayerPawn->GetActorLocation(), GetActorLocation());
}
