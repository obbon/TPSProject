
// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerFire.h"
#include "Camera/CameraComponent.h"
#include "Bullet.h"
#include "EnemyFSM.h"
#include "PlayerAnim.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"

UPlayerFire::UPlayerFire()
{
	ConstructorHelpers::FObjectFinder<USoundBase> tempSound(TEXT("SoundWave'/Game/SniperGun/Rifle.Rifle'"));
	if(tempSound.Succeeded())
	{
		bulletSound = tempSound.Object;
	}
}

void UPlayerFire::BeginPlay()
{
	Super::BeginPlay();
	
	tpsCamera = me->tpsCamera;
	gunMeshComp = me->gunMeshComp;
	sniperGunComp = me->sniperGunComp;
	sniperUI = CreateWidget(GetWorld(),sniperUICreator);
	crosshairUI = CreateWidget(GetWorld(),crosshairUICreator);
	crosshairUI->AddToViewport();
	ChangeToSniperGun();
}

void UPlayerFire::SetupInputBinding(class UInputComponent* PlayerInputComponent)
{
	PlayerInputComponent->BindAction(TEXT("Fire"),IE_Pressed,this,&UPlayerFire::InputFire);
	PlayerInputComponent->BindAction(TEXT("GrenadeGun"),IE_Pressed, this, &UPlayerFire::ChangeToGrenadeGun);
	PlayerInputComponent->BindAction(TEXT("SniperGun"), IE_Pressed, this, &UPlayerFire::ChangeToSniperGun);
	PlayerInputComponent->BindAction(TEXT("SniperMode"), IE_Pressed, this, &UPlayerFire::SniperMode);
	PlayerInputComponent->BindAction(TEXT("SniperMode"),IE_Released, this, &UPlayerFire::SniperMode);
}

void UPlayerFire::InputFire()
{
	UGameplayStatics::PlaySound2D(GetWorld(),bulletSound);
	auto controller = GetWorld()->GetFirstPlayerController();
	controller->PlayerCameraManager->StartCameraShake(cameraShake);
	auto anim = Cast<UPlayerAnim>(me->GetMesh()->GetAnimInstance());
	anim->PlayAttackAnim();
	if(bUsingGrenadeGun)
	{
		FTransform firePosition = gunMeshComp->GetSocketTransform(TEXT("FirePosition"));
		GetWorld()->SpawnActor<ABullet>(bulletFactory,firePosition);
	}else
	{
		FVector startPos = tpsCamera->GetComponentLocation();
		FVector endPos = tpsCamera->GetComponentLocation()+ tpsCamera->GetForwardVector() * 5000;
		FHitResult hitInfo;
		FCollisionQueryParams params;
		params.AddIgnoredActor(me);
		bool bhit = GetWorld()->LineTraceSingleByChannel(hitInfo, startPos, endPos,ECC_Visibility,params);
		
		if(bhit)
		{
			//충돌처리->총알 파편 효과 재생
			FTransform bulletTrans;
			bulletTrans.SetLocation(hitInfo.ImpactPoint);
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(),bulletEffectCreator,bulletTrans);

			auto hitComp = hitInfo.GetComponent();
			if(hitComp && hitComp->IsSimulatingPhysics())
			{
				FVector force = -hitInfo.ImpactNormal * hitComp->GetMass() * 20000;
				hitComp->AddForce(force);
			}
			auto enemy = hitInfo.GetActor()->GetDefaultSubobjectByName(TEXT("FSM"));
			if(enemy)
			{
				auto enemyFSM = Cast<UEnemyFSM>(enemy);
				enemyFSM->OnDemageProcess();
			}
		}
	}
	
}
void UPlayerFire::ChangeToGrenadeGun()
{
	bUsingGrenadeGun =true;
	sniperGunComp->SetVisibility(false);
	gunMeshComp->SetVisibility(true);
	
}

void UPlayerFire::ChangeToSniperGun()
{
	bUsingGrenadeGun =false;
	sniperGunComp->SetVisibility(true);
	gunMeshComp->SetVisibility(false);
}

void UPlayerFire::SniperMode()
{
	if(bUsingGrenadeGun)
	{
		return;
	}
	if(bSniperMode==false)
	{
		bSniperMode = true;
		sniperUI->AddToViewport();
		tpsCamera->SetFieldOfView(45.0f);
		crosshairUI->RemoveFromParent();
	}
	else
	{
		bSniperMode =false;
		sniperUI->RemoveFromParent();
		tpsCamera->SetFieldOfView(90.0f);
		crosshairUI->AddToViewport(); 
	}
}


