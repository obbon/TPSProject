// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "EnemyFSM.generated.h"

UENUM()
enum class EEnemyState : uint8
{
	Idle,Move, Attack, Damage, Die,
};
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TPSPROJECT_API UEnemyFSM : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UEnemyFSM();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category=FSM)
	EEnemyState eState = EEnemyState::Idle;

	UPROPERTY(EditDefaultsOnly, Category=FSM)
	float idleDelayTime = 2;
	float currentTime =0;
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category="FSM")
	class ATPSPlayer* targetPlayer;
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category="FSM")
	class AEnemy* enemy;
	UPROPERTY(EditAnywhere, Category=FSM)
	float attckRange = 150.0f;
	UPROPERTY(EditAnywhere, Category=FSM)
	float attackDelayTime = 2.0f;
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category=FSM)
	int32 hp = 3;
	UPROPERTY(EditAnywhere, Category=FSM)
	float damageDelayTime = 2.0f;
	UPROPERTY(EditAnywhere, Category=FSM)
	float dieSpeed = 500.0f;
	UPROPERTY()
	class UEnemyAnim* anim;
	UPROPERTY()
	class AAIController* ai;
	FVector randomPos;
	
	bool GetRandomPositionInNavMesh(FVector centerLoction, float radius, FVector& dest);
	void IdleState();
	void MoveState();
	void AttackState();
	void DamageState();
	void DieState();
	void OnDemageProcess();

	
};
