// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyFSM.h"

#include "AIController.h"
#include "Enemy.h"
#include "EnemyAnim.h"
#include "NavigationSystem.h"
#include "TPSPlayer.h"
#include "TPSProject.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values for this component's properties
UEnemyFSM::UEnemyFSM()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UEnemyFSM::BeginPlay()
{
	Super::BeginPlay();

	auto actor = UGameplayStatics::GetActorOfClass(GetWorld(), ATPSPlayer::StaticClass());
	targetPlayer =Cast<ATPSPlayer>(actor);
	enemy = Cast<AEnemy>(GetOwner());
	anim = Cast<UEnemyAnim>(enemy->GetMesh()->GetAnimInstance());
	ai = Cast<AAIController>(enemy->GetController());
}


// Called every frame
void UEnemyFSM::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	switch (eState)
	{
	case EEnemyState::Idle:
		IdleState();
		break;
	case EEnemyState::Move:
		MoveState();
		break;
	case EEnemyState::Attack:
		AttackState();
		break;
	case EEnemyState::Damage:
		DamageState();
		break;
	case EEnemyState::Die:
		DieState();
		break;
		
	}
}

bool UEnemyFSM::GetRandomPositionInNavMesh(FVector centerLoction, float radius, FVector& dest)
{
	auto ns = UNavigationSystemV1::GetNavigationSystem(GetWorld());
	FNavLocation loc;
	bool resultPosition = ns->GetRandomReachablePointInRadius(centerLoction, radius, loc);
	dest = loc.Location;
	return resultPosition;
}

void UEnemyFSM::IdleState()
{
	currentTime += GetWorld()->DeltaTimeSeconds;
	if(currentTime >idleDelayTime )
	{
		eState =EEnemyState::Move;
		currentTime =0;
		anim->animState = eState;
		GetRandomPositionInNavMesh(enemy->GetActorLocation(),500, randomPos);
	}
}

void UEnemyFSM::MoveState()
{
	FVector destination = targetPlayer->GetActorLocation();
	FVector dir = destination - enemy->GetActorLocation();
	enemy->AddMovementInput(dir.GetSafeNormal());
	auto ns = UNavigationSystemV1::GetNavigationSystem(GetWorld());
	FPathFindingQuery _isFind;
	FAIMoveRequest findReq;
	findReq.SetAcceptanceRadius(3);
	findReq.SetGoalLocation(destination);
	ai->BuildPathfindingQuery(findReq,_isFind);
	FPathFindingResult find = ns->FindPathSync(_isFind);
	if(find.Result == ENavigationQueryResult::Success)
	{
		ai->MoveToLocation(destination);
	}else
	{
		auto result = ai->MoveToLocation(randomPos);
		if(result==EPathFollowingRequestResult::AlreadyAtGoal)
		{
			GetRandomPositionInNavMesh(enemy->GetActorLocation(),500, randomPos);
		}
	}
	enemy->AddMovementInput(dir.GetSafeNormal());
	if(dir.Size()<attckRange)
	{
		ai->StopMovement();
		eState = EEnemyState::Attack;
		anim->animState = eState;
		anim->bAttackPlay = true;
		currentTime = attackDelayTime;
	}
}

void UEnemyFSM::AttackState()
{
	currentTime += GetWorld()->DeltaTimeSeconds;
	if(currentTime>attackDelayTime)
	{
		PRINT_LOG(TEXT("attack!!"));
		currentTime = 0;
		anim->bAttackPlay = true;
	}
	float distance = FVector::Distance(targetPlayer->GetActorLocation(), enemy->GetActorLocation());
	if(distance>attckRange)
	{
		eState = EEnemyState::Move;
		anim->animState = eState;
		GetRandomPositionInNavMesh(enemy->GetActorLocation(),500, randomPos);		
	}
}

void UEnemyFSM::DamageState()
{
	currentTime+= GetWorld()->DeltaTimeSeconds;
	if(currentTime > damageDelayTime)
	{
		eState = EEnemyState::Idle;
		currentTime = 0;
		anim->animState = eState;
	}
}

void UEnemyFSM::DieState()
{
	if(anim->bDieDone ==false)
	{
		return;
	}
	FVector p = enemy->GetActorLocation() + FVector::DownVector * dieSpeed * GetWorld()->DeltaTimeSeconds;
	enemy->SetActorLocation(p);
	if(p.Z < -200.0f)
	{
		enemy->Destroy();
	}
}

void UEnemyFSM::OnDemageProcess()
{
	hp--;
	if(hp>0)
	{
		eState = EEnemyState::Damage;
		currentTime =0;
		int32 index = FMath::RandRange(0,1);
		FString sectionName = FString::Printf(TEXT("Damage%d"),index);
		anim->PlayDamageAnim(FName(*sectionName));
	}else
	{
		eState = EEnemyState::Die;
		enemy->GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		anim->PlayDamageAnim(TEXT("Die"));
	}
	anim->animState = eState;
	ai->StopMovement();
}

