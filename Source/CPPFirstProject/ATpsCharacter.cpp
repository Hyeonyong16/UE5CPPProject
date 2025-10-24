// Fill out your copyright notice in the Description page of Project Settings.


#include "ATpsCharacter.h"

#include "PlayerData.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/TimelineComponent.h"
#include "InputMappingContext.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"

// Sets default values
AATpsCharacter::AATpsCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Camera Setting
	ThirdPersonSpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	ThirdPersonSpringArmComponent->SetupAttachment(GetCapsuleComponent());
	ThirdPersonSpringArmComponent->bUsePawnControlRotation = true;
	ThirdPersonSpringArmComponent->TargetArmLength = 200.f;
	ThirdPersonSpringArmComponent->SocketOffset.Set(0.f, 50.f, 60.f);

	ThirdPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	ThirdPersonCameraComponent->SetupAttachment(ThirdPersonSpringArmComponent);

	// 향상된 입력 시스템 설정
	static ConstructorHelpers::FObjectFinder<UInputMappingContext> ThirdPersonContextRef(
		TEXT("/Game/Blueprints/Input/IMC_Character.IMC_Character")
	);
	if (ThirdPersonContextRef.Succeeded())
	{
		ThirdPersonContext = ThirdPersonContextRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> MoveActionRef(
		TEXT("/Game/Blueprints/Input/IA_Move.IA_Move")
	);
	if (MoveActionRef.Succeeded())
	{
		MoveAction = MoveActionRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> JumpActionRef(
		TEXT("/Game/Blueprints/Input/IA_Jump.IA_Jump")
	);
	if (JumpActionRef.Succeeded())
	{
		JumpAction = JumpActionRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> LookActionRef(
		TEXT("/Game/Blueprints/Input/IA_Look.IA_Look")
	);
	if (LookActionRef.Succeeded())
	{
		LookAction = LookActionRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> AimActionRef(
		TEXT("/Game/Blueprints/Input/IA_Aiming.IA_Aiming")
	);
	if (AimActionRef.Succeeded())
	{
		AimAction = AimActionRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> FireActionRef(
		TEXT("/Game/Blueprints/Input/IA_Fire.IA_Fire")
	);
	if (FireActionRef.Succeeded())
	{
		FireAction = FireActionRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> InventoryActionRef(
		TEXT("/Game/Blueprints/Input/IA_Inventory.IA_Inventory")
	);
	if (InventoryActionRef.Succeeded())
	{
		InventoryAction = InventoryActionRef.Object;
	}

	// 플레이어 데이터 설정
	PlayerData = CreateDefaultSubobject<UPlayerData>(TEXT("PlayerData"));

	// 타임라인에 쓰일 커브 설정
	static ConstructorHelpers::FObjectFinder<UCurveFloat> Curve(
		TEXT("/Script/Engine.CurveFloat'/Game/Blueprints/Character/CB_Aim.CB_Aim'")
	);
	if (Curve.Succeeded())
	{
		AimCurve = Curve.Object;
	}

	// 인벤토리 클래스 설정
	static ConstructorHelpers::FClassFinder<UUserWidget> InventoryHUDClassRef(
		TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/Blueprints/UI/WBP_Inventory.WBP_Inventory_C'")
	);
	if (InventoryHUDClassRef.Succeeded())
	{
		InventoryHUDClass = InventoryHUDClassRef.Class;
	}
}

// Called when the game starts or when spawned
void AATpsCharacter::BeginPlay()
{
	Super::BeginPlay();

	// 글로벌 엔진 포인터가 null 인지 확인
	check(GEngine != nullptr);

	// 입력 매핑을 캐릭터에 바인딩
	// Controller 를 AplayerController 로 캐스팅
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		// 
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem
			= ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			// InputMappingContext 를 Subsystem 에 binding
			Subsystem->AddMappingContext(ThirdPersonContext, 0);
		}
	}

	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("Using ThirdPersonContext"));

	// Aim 용 TimeLine 추가
	if (AimCurve)
	{
		ProgressUpdate.BindUFunction(this, FName("AimUpdate")); // CallBack Func 바인딩

		// TimeLine 에 Curve 와 CallBack Func 추가
		AimTimeLine.AddInterpFloat(AimCurve, ProgressUpdate);
		AimTimeLine.SetTimelineLength(0.1f);
		AimTimeLine.SetLooping(false);
	}

	FireTimerDelegate.BindLambda([this]
	{
		IsFire = false;
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("IsFire Setting"));
	});

	// 인벤토리 위젯 생성
	InventoryHUD = CreateWidget(GetWorld()->GetFirstPlayerController(), InventoryHUDClass);
}

// Called every frame
void AATpsCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	AimTimeLine.TickTimeline(DeltaTime);
}

// 캐릭터에 일반 입력 컴포넌트가 있는지만 확인
// Called to bind functionality to input
void AATpsCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// 향상된 입력 컴포넌트가 있는지 확인해야 하므로 부모의 함수 호출 삭제할 것
	//Super::SetupPlayerInputComponent(PlayerInputComponent);

	// PlayerInputComponent 를 UEnhancedInputComponent 로 캐스팅
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		// IA_MOVE 가 호출되면 Move() 함수를 호출하도록 바인딩
		// MoveAction		: 캐릭터의 헤더파일에 선언된 바인딩할 입력 액션 (블루프린트에서 IA_MOVE 를 넣어줄거임)
		// ETriggerEvent	: 이벤트의 트리거 타입
		// this				: 바인딩할 대상 캐릭터 (여기선 직접 만든 TPSCharacter)
		// Move()			: 바인딩 하려는 함수에 대한 레퍼런스
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AATpsCharacter::Move);

		// 점프 액션 바인딩
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		// 카메라 이동 바인딩
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AATpsCharacter::Look);

		// Aim 액션 바인딩
		EnhancedInputComponent->BindAction(AimAction, ETriggerEvent::Started, this, &AATpsCharacter::Aim);
		EnhancedInputComponent->BindAction(AimAction, ETriggerEvent::Completed, this, &AATpsCharacter::StopAiming);

		// Fire 액션 바인딩
		EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Triggered, this, &AATpsCharacter::Fire);

		// Inventory 액션 바인딩
		EnhancedInputComponent->BindAction(InventoryAction, ETriggerEvent::Triggered, this, &AATpsCharacter::Inventory);
	}
}

void AATpsCharacter::Move(const FInputActionValue& Value)
{
	// inputAction 으로부터 움직임의 2D Vector 값을 받음
	const FVector2D MovementValue = Value.Get<FVector2D>();

	// 현재 액터가 소유한 컨트롤러에 대한 포인터가 유효한지 확인
	if (Controller != nullptr)
	{
		// 앞, 뒤 움직임
		FRotator ForwardRotator(0, GetControlRotation().Yaw, 0);
		AddMovementInput(UKismetMathLibrary::GetForwardVector(ForwardRotator) * MovementValue.Y);

		// 좌, 우 움직임
		FRotator RightRotator = GetControlRotation();
		const FVector Right = UKismetMathLibrary::GetRightVector(RightRotator);
		AddMovementInput(Right * MovementValue.X);


		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("Move"));
	}
}

void AATpsCharacter::Look(const FInputActionValue& Value)
{
	// inputAction 으로부터 움직임의 2D Vector 값을 받음
	const FVector2D LookAxisValue = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		AddControllerYawInput(LookAxisValue.X);
		AddControllerPitchInput(LookAxisValue.Y * -1);
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("Look"));
	}
}

void AATpsCharacter::Aim(const FInputActionValue& Value)
{
	if (CurrentPlayerState == EPlayerState::Idle)
	{
		CurrentPlayerState = EPlayerState::Aiming;
		IsAiming = true;

		GetCharacterMovement()->bOrientRotationToMovement = false;
		GetCharacterMovement()->MaxWalkSpeed; // 속도 지정해줘야함

		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("AimStart"));

		AimTimeLine.PlayFromStart(); // 타임라인 재생
	}
}

void AATpsCharacter::StopAiming(const FInputActionValue& Value)
{
	if (CurrentPlayerState == EPlayerState::Aiming)
	{
		CurrentPlayerState = EPlayerState::Idle;
	}
	else
	{
		if (CurrentPlayerState == EPlayerState::Inventory)
		{
			if (IsAiming != true)
			{
				return;
			}
		}
		else
		{
			return;
		}
	}

	IsAiming = false;

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->MaxWalkSpeed; // 속도 지정해줘야함

	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("AimEnd"));

	AimTimeLine.ReverseFromEnd(); // 타임라인 재생
}

void AATpsCharacter::Fire(const FInputActionValue& Value)
{
	// 인벤토리가 열려있으면 return
	if (CurrentPlayerState == EPlayerState::Inventory) { return; }

	// 재장전 중이면 return

	// 조준중이 아니면 return
	if (IsAiming == false) { return; }

	// 사격 딜레이 중이면 return
	if (IsFire == true) { return; }

	// 사격 딜레이 True
	IsFire = true;

	// timer 이벤트로 일정 시간 뒤 사격 딜레이 False
	GetWorld()->GetTimerManager().SetTimer(FireTimerHandle, FireTimerDelegate, 0.3f, false, 0.3f);

	// LineTrace 를 이용한 사격 구현
	FHitResult HitResult;
	FVector StartVector = ThirdPersonCameraComponent->GetComponentLocation();
	FVector EndVector = StartVector + (ThirdPersonCameraComponent->GetForwardVector() * 10000.f);

	// 본인은 라인트레이스에서 무시
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);

	bool bHit = GetWorld()->LineTraceSingleByChannel(
		HitResult,
		StartVector, EndVector,
		ECC_Visibility,
		Params
	);

	if (bHit)
	{
		// 에디터에서의 이름
		FString ActorName = HitResult.GetActor()->GetActorLabel();

		// Hit 지점을 구체로 표시
		DrawDebugSphere(GetWorld(), HitResult.ImpactPoint, 10.f, 12, FColor::Yellow, false, 2.f);
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Yellow, FString::Printf(TEXT("Hit Actor: %s"), *ActorName));
	}
}

void AATpsCharacter::Inventory()
{
	if (CurrentPlayerState == EPlayerState::Death) { return; }
	// 현재 인벤토리가 열려있으면
	if (InventoryHUD->IsInViewport())
	{
		// 인벤토리를 닫기
		// 부모에게서 HUD 제거
		InventoryHUD->RemoveFromParent();
		
		APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
		if (PlayerController)
		{
			// InputMode 를 GameOnly 로 변경
			FInputModeGameOnly Input;
			PlayerController->SetInputMode(Input);

			// 마우스 커서 표시하지 않기
			PlayerController->SetShowMouseCursor(false);

			// 플레이어 상태를 Inventory 에서 Idle 로 변경
			CurrentPlayerState = EPlayerState::Idle;
		}
	}
	
	// 현재 인벤토리가 닫혀있으면
	else
	{
		InventoryHUD->AddToViewport();

		APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
		if (PlayerController)
		{
			// InputMode 를 Game And UI 로 변경
			FInputModeGameAndUI	Input;

			// 포커스를 받을 위젯 설정
			Input.SetWidgetToFocus(InventoryHUD->TakeWidget());

			// 마우스 클릭 시 포커스가 UI 로 이동
			Input.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
			Input.SetHideCursorDuringCapture(false);

			// 플레이어 컨트롤러의 입력 모드 설정
			PlayerController->SetInputMode(Input);

			// 마우스 커서 보이게 설정
			PlayerController->SetShowMouseCursor(true);

			// 플레이어 상태를 Inventory 로 변경
			CurrentPlayerState = EPlayerState::Inventory;
		}
	}
}

void AATpsCharacter::AimUpdate(float Alpha)
{
	float CameraFOV = 90.f - (30.f * Alpha);
	ThirdPersonCameraComponent->SetFieldOfView(CameraFOV);

	float SpringArmLength = 200.f - (50.f * Alpha);
	ThirdPersonSpringArmComponent->TargetArmLength = SpringArmLength;
	FRotator MeshRelativeRot = FRotator(
		GetMesh()->GetRelativeRotation().Pitch,
		-90.f - (-12.f * Alpha),
		GetMesh()->GetRelativeRotation().Roll
	);
	GetMesh()->SetRelativeRotation(MeshRelativeRot);

	FVector SpringArmSocketOffset = FVector(
		ThirdPersonSpringArmComponent->SocketOffset.X,
		ThirdPersonSpringArmComponent->SocketOffset.Y,
		60.f + (20.f * Alpha)
	);
	ThirdPersonSpringArmComponent->SocketOffset = SpringArmSocketOffset;

	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("AimTimeline"));
}
