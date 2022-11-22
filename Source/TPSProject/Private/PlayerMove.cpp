// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerMove.h"

#include "GameFramework/CharacterMovementComponent.h"


void UPlayerMove::Turn(float value)
{
	me->AddControllerYawInput(value);
}

void UPlayerMove::LookUp(float value)
{
	me->AddControllerPitchInput(value);
}

UPlayerMove::UPlayerMove()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UPlayerMove::BeginPlay()
{
	Super::BeginPlay();
	moveComp->MaxWalkSpeed = walkSpeed;
}

void UPlayerMove::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	Move();
}

void UPlayerMove::SetupInputBinding(UInputComponent* PlayerInputComponent)
{
	PlayerInputComponent->BindAxis(TEXT("Turn"), this, &UPlayerMove::Turn);
	PlayerInputComponent->BindAxis(TEXT("LookUp"), this, &UPlayerMove::LookUp);
	PlayerInputComponent->BindAxis(TEXT("Horizontal"),this, &UPlayerMove::InputHorizontal);
	PlayerInputComponent->BindAxis(TEXT("Vertical"), this, &UPlayerMove::InputVertical);
	PlayerInputComponent->BindAction(TEXT("Jump"),IE_Pressed, this, &UPlayerMove::InputJump);
	PlayerInputComponent->BindAction(TEXT("Run"),IE_Pressed, this, &UPlayerMove::InputRun);
	PlayerInputComponent->BindAction(TEXT("Run"),IE_Released, this, &UPlayerMove::InputRun);
}
void UPlayerMove::InputHorizontal(float value)
{
	//direction.Y = value;
	if ((me->Controller!=NULL) && (value != 0.0f))
	{
		const FRotator rot = me->Controller->GetControlRotation();
		const FRotator yawRot(0,rot.Yaw,0);
		const FVector dir = FRotationMatrix(yawRot).GetUnitAxis(EAxis::Y);
		me->AddMovementInput(dir,value);
	}
}

void UPlayerMove::InputVertical(float value)
{
	//direction.X = value;
	if ((me->Controller!=NULL) && (value != 0.0f))
	{
		const FRotator rot = me->Controller->GetControlRotation();
		const FRotator yawRot(0,rot.Yaw,0);
		const FVector dir = FRotationMatrix(yawRot).GetUnitAxis(EAxis::X);
		me->AddMovementInput(dir,value);
	}
}

void UPlayerMove::InputJump()
{
	//direction.X = value;

	me->Jump();

}

void UPlayerMove::Move()
{
	direction = FTransform(me->GetControlRotation()).TransformVector(direction);
	// FVector newLocation = GetActorLocation() + direction* walkSpeed*DeltaTime;
	// SetActorLocation(newLocation);
	me->AddMovementInput(direction);
	direction = FVector::ZeroVector;
}

void UPlayerMove::InputRun()
{
	auto movement = moveComp;
	if(movement->MaxWalkSpeed>walkSpeed)
	{
		movement->MaxWalkSpeed = walkSpeed;
	}else
	{
		movement->MaxWalkSpeed = runSpeed;
	}
}

