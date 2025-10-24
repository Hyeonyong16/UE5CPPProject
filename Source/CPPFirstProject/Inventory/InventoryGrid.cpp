// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryGrid.h"

#include "InventoryComponent.h"
#include "Components/CanvasPanelSlot.h"

void UInventoryGrid::InitializeGrid(AInventoryComponent* InInventoryComponent, float InTileSize)
{
	InventoryComponent = InInventoryComponent;
	TileSize = InTileSize;

	UCanvasPanelSlot* GridCanvasPanelSlot = Cast<UCanvasPanelSlot>(GridBorder->Slot);
	if (GridCanvasPanelSlot)
	{
		float SizeX = InventoryComponent->Columns * TileSize;
		float SizeY = InventoryComponent->Rows * TileSize;
		GridCanvasPanelSlot->SetSize(FVector2D(SizeX, SizeY));

		// InventoryGrid 의 Line Clear 부분부터 해야함
	}
	
}
