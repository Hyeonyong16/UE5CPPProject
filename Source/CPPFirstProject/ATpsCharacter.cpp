// Fill out your copyright notice in the Description page of Project Settings.


#include "ATpsCharacter.h"

#include "PlayerData.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/TimelineComponent.h"
#include "Kismet/KismetMathLibrary.h"

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

	PlayerData = CreateDefaultSubobject<UPlayerData>(TEXT("PlayerData"));
	
	static ConstructorHelpers::FObjectFinder<UCurveFloat> Curve(
		TEXT("/Script/Engine.CurveFloat'/Game/Blueprints/Character/CB_Aim.CB_Aim'")
	);
	if (Curve.Succeeded())
	{
		AimCurve = Curve.Object;
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

		AimTimeLine.PlayFromStart();	// 타임라인 재생
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
	
	AimTimeLine.ReverseFromEnd();	// 타임라인 재생
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

