// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "../Struct/MyStructures.h"
#include "InventoryGrid.generated.h"

class UInventoryComponent;
/**
 * 
 */
UCLASS()
class CPPFIRSTPROJECT_API UInventoryGrid : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;
	
public:
	// 인벤토리 그리드 초기화 함수
	UFUNCTION()
	void InitializeGrid(UInventoryComponent* InventoryComponent, float TileSize);

protected:
	virtual int32 NativePaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry,
	                          const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements,
	                          int32 LayerId, const FWidgetStyle& InWidgetStyle,
	                          bool bParentEnabled
	) const override;

private:
	// 인벤토리에 그릴 라인의 정보를 생성하는 함수
	UFUNCTION()
	void CreateLineSegments();

protected:
	// 플레이어의 인벤토리 컴포넌트
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInventoryComponent> InventoryComponent;

	// 그려질 타일의 사이즈
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components", meta = (AllowPrivateAccess = "true"))
	float TileSize;

	// 그려질 그리드의 라인 정보
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components", meta = (AllowPrivateAccess = "true"))
	TArray<FLine> Lines;


	// Widget 바인딩용 변수 섹션
protected:
	UPROPERTY(BlueprintReadOnly, Category = "PANEL", meta = (BindWidget, AllowPrivateAccess = "true"))
	TObjectPtr<class UCanvasPanel> GridCanvasPanel;

	UPROPERTY(BlueprintReadOnly, Category = "BORDER", meta = (BindWidget, AllowPrivateAccess = "true"))
	TObjectPtr<class UBorder> GridBorder;
};
