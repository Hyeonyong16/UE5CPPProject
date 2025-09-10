// Fill out your copyright notice in the Description page of Project Settings.


#include "MyActor.h"

// Sets default values
AMyActor::AMyActor()
	: TotalDamage(400), DamageTimeInSeconds(2.5f)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	DamagePerSecond = TotalDamage / DamageTimeInSeconds;
	CharacterName = TEXT("Nickname");
	bAttackable = true;
}

// Called when the game starts or when spawned
void AMyActor::BeginPlay()
{
	Super::BeginPlay();
	
	UE_LOG(LogTemp, Log, TEXT("BeginPlay"));
}

// Called every frame
void AMyActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	UE_LOG(LogTemp, Log, TEXT("Tick"));
}

void AMyActor::CalculateDPS()
{
	DamagePerSecond = TotalDamage / DamageTimeInSeconds;
}

// 변수값이 초기화되면 호출되는 함수
void AMyActor::PostInitProperties()
{
	Super::PostInitProperties();

	CalculateDPS();
}

// 변수 값이 바뀌면 호출되는 함수
void AMyActor::PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent)
{
	CalculateDPS();
	
	Super::PostEditChangeProperty(PropertyChangedEvent);
}

void AMyActor::InputSpaceKey(const bool Pressed)
{
	bPressedKey = Pressed;
}

void AMyActor::MoveUp(float DeltaTime)
{
	if (bPressedKey)
	{
		FVector Location = GetActorLocation();
		Location += FVector::UpVector * 980.0f * DeltaTime;
		SetActorLocation(Location);
	}
}

