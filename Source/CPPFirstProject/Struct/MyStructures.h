// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MyStructures.generated.h"

USTRUCT(BlueprintType)
struct FLine
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector2D Start;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector2D End;
};

USTRUCT(BlueprintType)
struct FTile
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 X;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Y;
};

/**
 * 
 */
class CPPFIRSTPROJECT_API MyStructures
{
public:
	MyStructures();
	~MyStructures();
};
