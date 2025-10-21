// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerData.h"

#include "ATpsCharacter.h"

// Sets default values for this component's properties
UPlayerData::UPlayerData()
	: HP(0.f), MaxHP(0.f), Speed(0.f), AimingSpeed(0.f), BaseDamage(0.f), Bullets(0), MaxBullets(0)
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UPlayerData::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UPlayerData::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

