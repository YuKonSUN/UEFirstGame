// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AIController.h"
#include "Navigation/PathFollowingComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Components/CapsuleComponent.h"
#include "Components/ArrowComponent.h"
#include "Animation/AnimInstance.h"
#include <Runtime/AIModule/Classes/AITypes.h>

#include "XP_EnemyCharacter.generated.h"


UCLASS()
class FIRSTGAMECPP_API AXP_EnemyCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AXP_EnemyCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	//float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

public:

	UFUNCTION()
	void OnAIMoveCompleted(FAIRequestID RequestID, EPathFollowingResult::Type Result);

private:

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Animation", meta = (AllowPrivateAccess = "true"))
	class UAnimMontage* PushMontage;


	UPROPERTY(EditDefaultsOnly)
	UArrowComponent* AttackTracePos;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack", meta = (AllowPrivateAccess = "true"))
	float AcceptanceRadius = 5.0;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack", meta = (AllowPrivateAccess = "true"))
	USoundBase* DamageSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack", meta = (AllowPrivateAccess = "true"))
	float BaseDamage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack", meta = (AllowPrivateAccess = "true"))
	float AttackRadius;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health", meta = (AllowPrivateAccess = "true"))
	float Health;

	bool bCanAttack;

	

	// »Øµ÷º¯Êý
	UFUNCTION()
	void OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted);

	UFUNCTION()
	void OnNotifyBegin(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPointPayload);

	////UPROPERTY(EditDefaultsOnly)



protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float MoveSpeed = 100.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float TurnSpeed = 100.f;

private:

	void ChasePlayer();

	

};
