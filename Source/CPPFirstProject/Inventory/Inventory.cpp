// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory.h"

void UInventory::NativeConstruct()
{
	Super::NativeConstruct();

	
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("NativePaintInventory"));
}
