// Fill out your copyright notice in the Description page of Project Settings.


#include "XP_EnemyCharacter.h"
#include <FunctionalAITest.generated.h>

//#include <FunctionalAITest.generated.h>

// Sets default values
AXP_EnemyCharacter::AXP_EnemyCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	AttackTracePos = CreateDefaultSubobject<UArrowComponent>(TEXT("Attack Trace Pos"));
	AttackTracePos->SetupAttachment(GetCapsuleComponent());

	BaseDamage = 10.0f;
	AttackRadius = 25.0f;
	bCanAttack = true;
	Health = 20.0f;  // 假设初始生命值为100
}

// Called when the game starts or when spawned
void AXP_EnemyCharacter::BeginPlay()
{
	Super::BeginPlay();
	ChasePlayer();
	
}

// Called every frame
void AXP_EnemyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AXP_EnemyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AXP_EnemyCharacter::OnAIMoveCompleted(FAIRequestID RequestID, EPathFollowingResult::Type Result)
{
    if (Result == EPathFollowingResult::Success)
    {
		if (bCanAttack && PushMontage)
		{
			bCanAttack = false;

			//播放攻击动画
			UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();

			if (AnimInstance)
			{

				FScriptDelegate BeginDelegateSubscriber;
				BeginDelegateSubscriber.BindUFunction(this, "OnAttackMontageEnded");
				AnimInstance->OnMontageEnded.Add(BeginDelegateSubscriber);

				FScriptDelegate NotifyBeginDelegateSubscriber;
				NotifyBeginDelegateSubscriber.BindUFunction(this, "OnNotifyBegin");
				AnimInstance->OnPlayMontageNotifyBegin.Add(NotifyBeginDelegateSubscriber);


				AnimInstance->Montage_Play(PushMontage);

			}
		}

		
    }
}

void AXP_EnemyCharacter::OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	if (Montage == PushMontage)
	{
		bCanAttack = true;

		ChasePlayer();
	}
}

void AXP_EnemyCharacter::OnNotifyBegin(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPointPayload)
{
	// 获取攻击位置和方向
	FVector Start = AttackTracePos->GetComponentLocation();
	FVector ForwardVector = AttackTracePos->GetForwardVector();
	FVector End = Start + (ForwardVector * 100.0f);

	// 设置攻击判定参数
	TArray<AActor*> IgnoredActors;
	IgnoredActors.Add(this);

	FHitResult HitResult;
	bool bHit = UKismetSystemLibrary::SphereTraceSingle(this, Start, End, AttackRadius, ETraceTypeQuery::TraceTypeQuery1, false, IgnoredActors, EDrawDebugTrace::ForDuration, HitResult, true);

	if (bHit)
	{
		// 处理攻击命中逻辑
		AActor* HitActor = HitResult.GetActor();
		if (HitActor)
		{
			UGameplayStatics::ApplyDamage(HitActor, BaseDamage, GetController(), this, nullptr);
		}
	}

}

void AXP_EnemyCharacter::ChasePlayer()
{
    APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
    APawn* PlayerPawn = PlayerController ? PlayerController->GetPawn() : nullptr;

    if (PlayerPawn)
    {
        AAIController* AIController = Cast<AAIController>(GetController());
        if (AIController)
        {
            //FNavPathSharedPtr NavPath;
            
            AIController->MoveToActor(PlayerPawn, AcceptanceRadius, true, true, false, 0, true);
            
            AIController->ReceiveMoveCompleted.AddDynamic(this, &AXP_EnemyCharacter::OnAIMoveCompleted);
            
        }
    }
}

