// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerAnimInstance.h"
#include "ATpsCharacter.h"
#include "KismetAnimationLibrary.h"

UPlayerAnimInstance::UPlayerAnimInstance()
{
	Speed = 0.f;
}

void UPlayerAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	// 폰에 접근
	auto Pawn = TryGetPawnOwner();
	// 접근한 폰 객체가 유효한지 확인
	if (IsValid(Pawn))
	{
		// 접근한 폰 객체가 AATpsCharacter 인지 Cast 를 통해서 확인
		if (AATpsCharacter* PlayerCharacter = Cast<AATpsCharacter>(Pawn))
		{
			Speed = PlayerCharacter->GetVelocity().Size();
			Direction = UKismetAnimationLibrary::CalculateDirection(
				PlayerCharacter->GetVelocity(),
				PlayerCharacter->GetActorRotation()
			);
		}
	}
}
