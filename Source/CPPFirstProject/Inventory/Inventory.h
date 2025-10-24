// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Inventory.generated.h"

/**
 * 
 */
UCLASS()
class CPPFIRSTPROJECT_API UInventory : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;
	
	// Widget 바인딩용 변수 섹션
protected:
	// 유저 인벤토리
	UPROPERTY(BlueprintReadOnly, Category = "InventoryGrid", meta = (BindWidget, AllowPrivateAccess = "true"))
	TObjectPtr<class UInventoryGrid> InventoryGridWidget;

	// 유저 인벤토리 박스

private:
	// 그려질 타일의 사이즈
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components", meta = (AllowPrivateAccess = "true"))
	float TileSize;
};
