// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerAnim.h"

#include "TPSPlayer.h"
#include "GameFramework/CharacterMovementComponent.h"

void UPlayerAnim::PlayAttackAnim()
{
	Montage_Play(attackAniMontage);
}

void UPlayerAnim::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	auto ownerPawn = TryGetPawnOwner();
	auto player = Cast<ATPSPlayer>(ownerPawn);
	if(player!=nullptr)
	{
		FVector velocity =player->GetVelocity();
		FVector forwardVector = player->GetActorForwardVector();
		speed = FVector::DotProduct(forwardVector,velocity);
		FVector rightVector = player->GetActorRightVector();
		direction = FVector::DotProduct(rightVector,velocity);

		auto movement = player->GetCharacterMovement();
		_isAir = movement->IsFalling();
	}
}
