// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "TPSPlayer.generated.h"



UCLASS()
class TPSPROJECT_API ATPSPlayer : public ACharacter
{
	GENERATED_BODY()

public:

	// Sets default values for this character's properties
	ATPSPlayer();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:


	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	UPROPERTY(VisibleAnywhere,Category=Camera)
	class USpringArmComponent* springArmComp;
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly, Category=Camera)
	class UCameraComponent* tpsCamera;
	UPROPERTY(VisibleAnywhere,Category=GunMesh)
	class USkeletalMeshComponent* gunMeshComp;
	UPROPERTY(VisibleAnywhere,Category=GunMesh)
	class UStaticMeshComponent * sniperGunComp;

public:
	UPROPERTY(VisibleAnywhere,Category=Component)
	class UPlayerBaseComponent* playerMove;
	UPROPERTY(VisibleAnywhere,Category=Component)
	class UPlayerBaseComponent* playerFire;
};
