// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryGrid.h"

#include "InventoryComponent.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Components/Border.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/CanvasPanel.h"
#include "CPPFirstProject/ATpsCharacter.h"


void UInventoryGrid::NativeConstruct()
{
	Super::NativeConstruct();

	AATpsCharacter* PlayerCharacter = Cast<AATpsCharacter>(GetOwningPlayerPawn());
	if (PlayerCharacter)
	{
		InitializeGrid(PlayerCharacter->InventoryComponent, PlayerCharacter->InventoryComponent->TileSize);
	}

	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("NativeConstruct"));
}

void UInventoryGrid::InitializeGrid(UInventoryComponent* InInventoryComponent, float InTileSize)
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
		Lines.Empty();

		// Line 정보 생성
		CreateLineSegments();
	}
}

int32 UInventoryGrid::NativePaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry,
                                  const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements,
                                  int32 LayerId,
                                  const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const
{
	int32 CurrentLayerID = Super::NativePaint(Args, AllottedGeometry, MyCullingRect, OutDrawElements, LayerId,
	                                          InWidgetStyle,
	                                          bParentEnabled);

	FPaintContext PaintContext(AllottedGeometry, MyCullingRect, OutDrawElements, LayerId, InWidgetStyle, bParentEnabled);
	FVector2D GridBorderTopLeft = GridBorder->GetCachedGeometry().GetLocalPositionAtCoordinates(FVector2D(0.f, 0.f));

	for (FLine GridLine : Lines)
	{
		UWidgetBlueprintLibrary::DrawLine(
			PaintContext,
			GridLine.Start + GridBorderTopLeft,
			GridLine.End + GridBorderTopLeft,
			FLinearColor(0.5f, 0.5f, 0.5f, 0.5f)
		);
	}
	
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("NativePaintGrid"));

	return int32();
}

void UInventoryGrid::CreateLineSegments()
{
	// 인벤토리의 수직선 생성
	for (int32 i = 0; i < InventoryComponent->Columns; ++i)
	{
		float X = i * TileSize;
		float Y = InventoryComponent->Rows * TileSize;
		FLine GridLine = FLine(
			FVector2D(X, 0.f),
			FVector2D(X, Y)
		);
		Lines.Add(GridLine);
	}
	
	// 인벤토리의 수평선 생성
	for (int32 i = 0; i < InventoryComponent->Rows; ++i)
	{
		float Y = i * TileSize;
		float X = InventoryComponent->Columns * TileSize;
		FLine GridLine = FLine(
			FVector2D(0.f, Y),
			FVector2D(X, Y)
		);
		Lines.Add(GridLine);
	}
	
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("CreateLineSegment"));
}
