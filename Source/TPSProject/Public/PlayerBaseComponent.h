// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TPSPlayer.h"
#include  "GameFramework/CharacterMovementComponent.h"
#include "Components/ActorComponent.h"
#include "PlayerBaseComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TPSPROJECT_API UPlayerBaseComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UPlayerBaseComponent()
	{
		PrimaryComponentTick.bCanEverTick = false;
	};
	virtual void BeginPlay() override;
	virtual void SetupInputBinding(class UInputComponent* PlayerInputComponent){}; 

public:
UPROPERTY()
	ATPSPlayer* me;
UPROPERTY()
	UCharacterMovementComponent* moveComp;

};
