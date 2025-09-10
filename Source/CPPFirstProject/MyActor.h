// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MyActor.generated.h"

UCLASS()
class CPPFIRSTPROJECT_API AMyActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMyActor();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Damage")
		int32 TotalDamage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Damage")
		float DamageTimeInSeconds;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Transient, Category="Damage")
		float DamagePerSecond;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString CharacterName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool bAttackable;

	bool bPressedKey;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	
	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void CalculateDPS();

	virtual void PostInitProperties() override;
	virtual void PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent) override;

	UFUNCTION(BlueprintCallable, Category = "Move")
		void InputSpaceKey(bool Pressed);

	UFUNCTION(BlueprintCallable, Category = "Move")
		void MoveUp(float DeltaTime);
};
