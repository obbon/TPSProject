// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "PlayerAnim.generated.h"



UCLASS()
class TPSPROJECT_API UPlayerAnim : public UAnimInstance
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category=PlayerAnim)
	float speed = 0;
	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite, Category=PlayerAnim)
	float direction = 0;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category=PlayerAnim)
	bool _isAir = false;

	UPROPERTY(EditDefaultsOnly, Category=PlayerAnim)
	class UAnimMontage* attackAniMontage;

	void PlayAttackAnim();
	
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
	
};
