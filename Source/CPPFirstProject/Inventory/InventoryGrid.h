// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Border.h"
#include "InventoryGrid.generated.h"

struct FLine;
class AInventoryComponent;
/**
 * 
 */
UCLASS()
class CPPFIRSTPROJECT_API UInventoryGrid : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UFUNCTION()
	void InitializeGrid(AInventoryComponent* InventoryComponent, float TileSize);

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components", meta = (AllowPrivateAccess = "true"))
	AInventoryComponent* InventoryComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components", meta = (AllowPrivateAccess = "true"))
	float TileSize;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components", meta = (AllowPrivateAccess = "true"))
	TArray<FLine> Lines;
	

	// Widget 바인딩용 변수 섹션
protected:
	UPROPERTY(BlueprintReadOnly, Category = "PANEL", meta = (BindWidget, AllowPrivateAccess = "true"))
	UCanvasPanel* GridCanvasPanel;

	UPROPERTY(BlueprintReadOnly, Category = "BORDER", meta = (BindWidget, AllowPrivateAccess = "true"))
	UBorder* GridBorder;
};
