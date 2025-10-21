// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "PlayerAnimInstance.generated.h"

/**
 *  Player의 Anim State
 *  
 */
UCLASS()
class CPPFIRSTPROJECT_API UPlayerAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	// 생성자 선언
	UPlayerAnimInstance();

	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

private:
	// AllowPrivateAccess : private에 선언된 변수를 블루프린트에서 접근할 수 있도록 설정
	// 플레이어 속도
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Pawn, meta=(AllowPrivateAccess=true))
	float Speed = 0.f;

	// 플레이어 방향
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Pawn, meta=(AllowPrivateAccess=true))
	float Direction = 0.f;
	
	
};
