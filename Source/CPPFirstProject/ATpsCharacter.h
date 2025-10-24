// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
// 향상된 입력 시스템
#include "EnhancedInputComponent.h"
#include "InputActionValue.h"
#include "EnhancedInputSubsystems.h"
// 카메라 추가
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/TimelineComponent.h"

#include "Struct/MyStructures.h"

// 얘가 include 중가장 마지막에 있어야함
#include "ATpsCharacter.generated.h"

UENUM(BlueprintType)
enum class EPlayerState : uint8
{
	Idle UMETA(DisplayName = "Idle"),
	Aiming UMETA(DisplayName = "Aiming"),
	Inventory UMETA(DisplayName = "Inventory"),
	Death UMETA(DisplayName = "Death"),
};

// 향상된 입력 시스템 사용을 위한 전방선언
class UInputMappingContext;
class UInputAction;
class UInputComponent;
class UCapsuleComponent;

class UInventoryComponent;
class UPlayerData;

UCLASS()
class CPPFIRSTPROJECT_API AATpsCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AATpsCharacter();

	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// 입력 매핑 컨텍스트 포인터
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UInputMappingContext> ThirdPersonContext;

	// 이동, 점프 Action 추가
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UInputAction> MoveAction;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UInputAction> JumpAction;

	// 카메라 이동 Look Action 추가
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UInputAction> LookAction;

	// 조준 Action 추가
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UInputAction> AimAction;

	// 사격 Action 추가
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UInputAction> FireAction;

	// 인벤토리 Action 추가
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UInputAction> InventoryAction;
	
	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// 입력 액션 값을 생성하고, 이 값을 사용하여 캐릭터 움직임을 적용하는 함수
	UFUNCTION()
	void Move(const FInputActionValue& Value);

	// 마우스 값으로 Look Input 을 적용하는 함수s
	UFUNCTION()
	void Look(const FInputActionValue& Value);

	// 우클릭 시 조준하는 함수
	UFUNCTION()
	void Aim(const FInputActionValue& Value);
	
	UFUNCTION()
	void StopAiming(const FInputActionValue& Value);

	UFUNCTION()
	void Fire(const FInputActionValue& Value);

	UFUNCTION()
	void Inventory();

	UPROPERTY(EditAnywhere, Category = "Camera")
	TObjectPtr<UCameraComponent> ThirdPersonCameraComponent;

	UPROPERTY(EditAnywhere, Category = "Camera")
	TObjectPtr<USpringArmComponent> ThirdPersonSpringArmComponent;

	UPROPERTY(EditAnywhere, Category = "PlayerStat")
	TObjectPtr<UPlayerData> PlayerData;
	
	UPROPERTY(EditAnywhere, Category = "Inventory")
	TObjectPtr<UInventoryComponent> InventoryComponent;
	
private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats", meta=(AllowPrivateAccess=true))
	EPlayerState CurrentPlayerState = EPlayerState::Idle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Flags", meta=(AllowPrivateAccess=true))
	bool IsAiming = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Flags", meta=(AllowPrivateAccess=true))
	bool IsFire = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Flags", meta=(AllowPrivateAccess=true))
	bool IsReloading = false;

	UPROPERTY(EditAnywhere, Category = "TimeLine")
	TObjectPtr<UCurveFloat> AimCurve;

	FTimeline AimTimeLine;
	
	UFUNCTION()
	void AimUpdate(float Alpha);

	FOnTimelineFloat ProgressUpdate;

	FTimerHandle FireTimerHandle;
	FTimerDelegate FireTimerDelegate;

	// 인벤토리 관련
protected:
	UPROPERTY(EditAnywhere, Category = "Inventory", meta = (AllowPrivateAccess = true))
	TSubclassOf<class UUserWidget> InventoryHUDClass;

	UPROPERTY()
	TObjectPtr<UUserWidget> InventoryHUD;
};
