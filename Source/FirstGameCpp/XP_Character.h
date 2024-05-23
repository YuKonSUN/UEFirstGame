// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Camera/CameraComponent.h"
#include "InputActionValue.h"
#include "GameFramework/Character.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/ArrowComponent.h"
#include "Engine/StaticMesh.h"
#include "Blueprint/UserWidget.h"

#include "XP_Character.generated.h"

UCLASS()
class FIRSTGAMECPP_API AXP_Character : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AXP_Character();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void Tick(float DeltaTime);

	void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent);

	float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

public:	

	

private:

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Animation", meta = (AllowPrivateAccess = "true"))
	class UAnimMontage* PushMontage;

	UPROPERTY(EditDefaultsOnly)
	UCameraComponent* PlayerCamera;

	UPROPERTY(EditDefaultsOnly)
	UArrowComponent* AttackTracePos;

	UPROPERTY(EditDefaultsOnly)
	class USpringArmComponent* SpringArmComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack", meta = (AllowPrivateAccess = "true"))
	USoundBase* DamageSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack", meta = (AllowPrivateAccess = "true"))
	float BaseDamage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack", meta = (AllowPrivateAccess = "true"))
	float AttackRadius;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health", meta = (AllowPrivateAccess = "true"))
	float Health;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health", meta = (AllowPrivateAccess = "true"))
	float MaxHealth;



	bool bCanAttack;

	// 回调函数
	UFUNCTION()
	void OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted);

	UFUNCTION()
	void OnNotifyBegin(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPointPayload);

	//UPROPERTY(EditDefaultsOnly)
	


protected:
	UPROPERTY(EditAnywhere, Category = "EnhancedInput")
	class UInputMappingContext* InputMapping;

	UPROPERTY(EditAnywhere, Category = "EnhancedInput")
	class UInputAction* MoveAction;

	UPROPERTY(EditAnywhere, Category = "EnhancedInput")
	class UInputAction* LookAction;

	UPROPERTY(EditAnywhere, Category = "EnhancedInput")
	class UInputAction* AttackAction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float MoveSpeed = 100.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float TurnSpeed = 100.f;

private:

	void Movement(const FInputActionValue& ActionValue);

	void Look(const FInputActionValue& ActionValue);

	void Attack();

	UAnimInstance* AnimInstance;

public:
	// 在这里声明函数
	void CreateHealthBarWidget();

	// 在这里声明变量
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UUserWidget> HealthBarWidgetClass;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UUserWidget> AttackHUDWidgetClass;

protected:

	UUserWidget* HealthBarWidget;

	//UPROPERTY(BlueprintReadWrite)
	UUserWidget* AttackHUDWidget;

	

};
