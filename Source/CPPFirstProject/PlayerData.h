// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PlayerData.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CPPFIRSTPROJECT_API UPlayerData : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UPlayerData();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	float HP;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	float MaxHP;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	float Speed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	float AimingSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	float BaseDamage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	int32 Bullets;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	int32 MaxBullets;
};
