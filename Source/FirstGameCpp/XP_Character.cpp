// Fill out your copyright notice in the Description page of Project Settings.


#include "XP_Character.h"
#include "InputMappingContext.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include <Kismet/KismetSystemLibrary.h>
#include <Kismet/GameplayStatics.h>
#include <Subsystems/PanelExtensionSubsystem.h>

// Sets default values
AXP_Character::AXP_Character()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	
	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComponent"));
	SpringArmComponent->SetupAttachment(GetCapsuleComponent());
	SpringArmComponent->TargetArmLength = 300.0f; // The camera follows at this distance behind the character
	SpringArmComponent->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	PlayerCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("PlayerCamera"));
	PlayerCamera->SetupAttachment(SpringArmComponent);
	PlayerCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm
	

	AttackTracePos = CreateDefaultSubobject<UArrowComponent>(TEXT("Attack Trace Pos"));
	AttackTracePos->SetupAttachment(GetCapsuleComponent());

	// Ĭ��ֵ
	BaseDamage = 10.0f;
	AttackRadius = 25.0f;
	bCanAttack = true;
	MaxHealth = 100.f;
	Health = 100.0f;  // �����ʼ����ֵΪ100
	
}

// Called when the game starts or when spawned
void AXP_Character::BeginPlay()
{
	Super::BeginPlay();
	AnimInstance = GetMesh()->GetAnimInstance();


	AnimInstance->OnMontageEnded.AddDynamic(this, &AXP_Character::OnAttackMontageEnded);



	AnimInstance->OnPlayMontageNotifyBegin.AddDynamic(this, &AXP_Character::OnNotifyBegin);

	CreateHealthBarWidget();
}

void AXP_Character::Movement(const FInputActionValue& ActionValue)
{
	FVector2D InputVector = ActionValue.Get<FVector2D>();

	// ��ȡ����������ת
	FRotator ControlRotation = Controller != nullptr ? Controller->GetControlRotation() : FRotator::ZeroRotator;
	// ��ʹ��Yaw��ƽ����ת��
	FRotator YawRotation(0, ControlRotation.Yaw, 0);

	// ���㷽������
	FVector ForwardVector = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	FVector RightVector = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	FVector MoveVector = ForwardVector * InputVector.Y + RightVector * InputVector.X;

	AddMovementInput(MoveVector, MoveSpeed * GetWorld()->GetDeltaSeconds());
}

void AXP_Character::Look(const FInputActionValue& ActionValue)
{
	FVector2D LookInput = ActionValue.Get<FVector2D>();
	AddControllerYawInput(LookInput.X * TurnSpeed * GetWorld()->GetDeltaSeconds());
	AddControllerPitchInput(-1.f * LookInput.Y * TurnSpeed * GetWorld()->GetDeltaSeconds());
}

void AXP_Character::Attack()
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

void AXP_Character::CreateHealthBarWidget()
{
	if (HealthBarWidgetClass)
	{
		UWorld* World = GetWorld();
		if (World)
		{
			APlayerController* PlayerController = World->GetFirstPlayerController();
			if (PlayerController)
			{
				HealthBarWidget = CreateWidget<UUserWidget>(World, HealthBarWidgetClass);
				if (HealthBarWidget)
				{
					//HealthBarWidget->AddToViewport();
					HealthBarWidget->AddToViewport();
					//PlayerController->bShowMouseCursor = true;
				}

				AttackHUDWidget = CreateWidget<UUserWidget>(World, AttackHUDWidgetClass);
				if (AttackHUDWidget)
				{
					//HealthBarWidget->AddToViewport();
					AttackHUDWidget->AddToViewport();
					//PlayerController->bShowMouseCursor = true;
				}

			}
		}
	}
}

// Called every frame
void AXP_Character::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AXP_Character::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (APlayerController* playerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(playerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(InputMapping, 0);
		}
	}

	if (UEnhancedInputComponent* Input = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		Input->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AXP_Character::Movement);
		Input->BindAction(LookAction, ETriggerEvent::Triggered, this, &AXP_Character::Look);
		Input->BindAction(AttackAction, ETriggerEvent::Triggered, this, &AXP_Character::Attack);
	}
}

void AXP_Character::OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	if (Montage == PushMontage)
	{
		bCanAttack = true;
	}
}

void AXP_Character::OnNotifyBegin(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPointPayload)
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
		EDrawDebugTrace::None,
		HitResult,
		true
	);

	/*FHitResult HitResult;
	bool bHit = UKismetSystemLibrary::SphereTraceSingleForObjects(this, Start, End, AttackRadius, ETraceTypeQuery::TraceTypeQuery1, false, IgnoredActors, EDrawDebugTrace::ForDuration, HitResult, true);*/
	
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

float AXP_Character::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
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

		// ��������
		APlayerController* PlayerController = Cast<APlayerController>(GetController());
		if (PlayerController)
		{
			DisableInput(PlayerController);
		}

		

		// ����ģ������
		//GetMesh()->SetSimulatePhysics(true);

		GetMesh()->SetSimulatePhysics(true); // ����ģ������
		GetMesh()->SetCollisionProfileName(TEXT("Custom")); // ������ײԤ��Ϊ�Զ���
		GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics); // ���ò�ѯ��������ײ
		GetMesh()->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block); // ����������ײͨ����Ĭ����Ӧ
		GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
		GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);

		bCanAttack = false;
	}
	else
	{
		// ִ�����������߼�
		FVector LaunchVelocity = GetActorForwardVector() * -200.0f + GetActorUpVector() * 100.0f;
		LaunchCharacter(LaunchVelocity, true, true);
	}

	return DamageAmount;
}

