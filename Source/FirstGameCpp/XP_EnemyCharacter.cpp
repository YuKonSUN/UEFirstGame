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
	Health = 20.0f;  // �����ʼ����ֵΪ100
}

// Called when the game starts or when spawned
void AXP_EnemyCharacter::BeginPlay()
{
	Super::BeginPlay();
	AnimInstance = GetMesh()->GetAnimInstance();

	AnimInstance->OnMontageEnded.AddDynamic(this, &AXP_EnemyCharacter::OnAttackMontageEnded);

	AnimInstance->OnPlayMontageNotifyBegin.AddDynamic(this, &AXP_EnemyCharacter::OnNotifyBegin);

	GetWorldTimerManager().SetTimer(TimerHandle_GetAIController, this, &AXP_EnemyCharacter::GetAIController, 0.5f, false);

	//AIController = Cast<AAIController>(GetController());
	////AIController->ReceiveMoveCompleted.AddDynamic(this, &AXP_EnemyCharacter::OnAIMoveCompleted);

	//if (AIController)
	//{
	//	AIController->ReceiveMoveCompleted.AddDynamic(this, &AXP_EnemyCharacter::OnAIMoveCompleted);
	//}
	//else
	//{
	//	UE_LOG(LogTemp, Error, TEXT("AIController is null in BeginPlay"));
	//}
	//ChasePlayer();
	
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

float AXP_EnemyCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	// ���ٽ�ɫ����ֵ
	Health -= DamageAmount;

	// ������������
	UGameplayStatics::PlaySoundAtLocation(this, DamageSound, GetActorLocation());

	// �������ֵС�ڵ���0�������ɫ�����߼�
	if (Health <= 0)
	{
		Health = 0;

		//// ��������
		//APlayerController* PlayerController = Cast<APlayerController>(GetController());
		//if (PlayerController)
		//{
		//	DisableInput(PlayerController);
		//}
		
		

		// ����ģ������
		GetMesh()->SetSimulatePhysics(true); // ����ģ������
		GetMesh()->SetCollisionProfileName(TEXT("Custom")); // ������ײԤ��Ϊ�Զ���
		GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics); // ���ò�ѯ��������ײ
		GetMesh()->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block); // ����������ײͨ����Ĭ����Ӧ
		GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
		GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);

		bCanAttack = false;

		if (UCharacterMovementComponent* MovementComponent = GetCharacterMovement())
		{
			MovementComponent->DisableMovement();
		}

		GetWorld()->GetTimerManager().SetTimer(DelayTimerHandle, this, &AXP_EnemyCharacter::CheckForEnemiesAndDestroy, 3.0f, false);
		
	}
	else
	{
		// ִ�����������߼�
		FVector LaunchVelocity = GetActorForwardVector() * -200.0f + GetActorUpVector() * 100.0f;
		LaunchCharacter(LaunchVelocity, true, true);
	}

	return DamageAmount;
}

void AXP_EnemyCharacter::OnAIMoveCompleted(FAIRequestID RequestID, EPathFollowingResult::Type Result)
{
    if (Result == EPathFollowingResult::Success)
    {
		if (bCanAttack && PushMontage)
		{
			bCanAttack = false;

			//���Ź�������
			

			if (AnimInstance)
			{

				AnimInstance->Montage_Play(PushMontage);

			}
		}

		
    }
}

void AXP_EnemyCharacter::CheckForEnemiesAndDestroy()
{
	
	Destroy();
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
	// ��ȡ����λ�úͷ���
	FVector Start = AttackTracePos->GetComponentLocation();
	FVector ForwardVector = AttackTracePos->GetForwardVector();
	FVector End = Start + (ForwardVector * 100.0f);

	// ���ù����ж�����
	TArray<AActor*> IgnoredActors;
	IgnoredActors.Add(this);

	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
	ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_Pawn));

	FHitResult HitResult;
	bool bHit = UKismetSystemLibrary::SphereTraceSingleForObjects(
		this,
		Start,
		End,
		AttackRadius,
		ObjectTypes,
		false,
		IgnoredActors,
		EDrawDebugTrace::ForDuration,
		HitResult,
		true
	);

	/*FHitResult HitResult;
	bool bHit = UKismetSystemLibrary::SphereTraceSingle(this, Start, End, AttackRadius, ETraceTypeQuery::TraceTypeQuery1, false, IgnoredActors, EDrawDebugTrace::ForDuration, HitResult, true);*/

	if (bHit)
	{
		// �����������߼�
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
        if (AIController)
        {
            //FNavPathSharedPtr NavPath;
            
            AIController->MoveToActor(PlayerPawn, AcceptanceRadius, true, true, false, 0, true);

            
        }
    }
}

void AXP_EnemyCharacter::GetAIController()
{
	AIController = Cast<AAIController>(GetController());

	if (AIController)
	{
		AIController->ReceiveMoveCompleted.AddDynamic(this, &AXP_EnemyCharacter::OnAIMoveCompleted);
		ChasePlayer();
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed to get AIController, retrying..."));
		GetWorldTimerManager().SetTimer(TimerHandle_GetAIController, this, &AXP_EnemyCharacter::GetAIController, 0.5f, false);
	}
}

