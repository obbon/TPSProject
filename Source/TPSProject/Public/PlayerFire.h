// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PlayerBaseComponent.h"
#include "PlayerFire.generated.h"

/**
 * 
 */
UCLASS()
class TPSPROJECT_API UPlayerFire : public UPlayerBaseComponent
{
	GENERATED_BODY()

public:
	UPlayerFire();

public:
	virtual void BeginPlay() override;
	virtual void SetupInputBinding(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY()
	class UCameraComponent* tpsCamera;
	UPROPERTY()
	class USkeletalMeshComponent* gunMeshComp;
	UPROPERTY()
	class UStaticMeshComponent* sniperGunComp;
	UPROPERTY(EditDefaultsOnly, Category=BulletFactory)
	TSubclassOf<class ABullet> bulletFactory;
	UPROPERTY(EditDefaultsOnly, Category=SniperUI)
	TSubclassOf<class UUserWidget> sniperUICreator;
	UPROPERTY(EditDefaultsOnly, Category=SniperUI)
	TSubclassOf<class UUserWidget> crosshairUICreator;
	UPROPERTY(EditAnywhere,Category=BulletEffect)
	class UParticleSystem* bulletEffectCreator;
	UPROPERTY(EditDefaultsOnly, Category=CameraMotion)
	TSubclassOf<UCameraShakeBase> cameraShake;
	UPROPERTY(EditDefaultsOnly, Category=Sound)
	class USoundBase* bulletSound;
	bool bSniperMode = false;
	bool bUsingGrenadeGun = true;
	class UUserWidget* sniperUI;
	class UUserWidget* crosshairUI;

	void InputFire();
	void ChangeToGrenadeGun();
	void ChangeToSniperGun();
	void SniperMode();
	
};
