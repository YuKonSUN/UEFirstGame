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

	// 默认值
	BaseDamage = 10.0f;
	AttackRadius = 25.0f;
	bCanAttack = true;
	MaxHealth = 100.f;
	Health = 100.0f;  // 假设初始生命值为100
	
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

	// 获取控制器的旋转
	FRotator ControlRotation = Controller != nullptr ? Controller->GetControlRotation() : FRotator::ZeroRotator;
	// 仅使用Yaw（平面旋转）
	FRotator YawRotation(0, ControlRotation.Yaw, 0);

	// 计算方向向量
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

		//播放攻击动画

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
	// 获取攻击位置和方向
	FVector Start = AttackTracePos->GetComponentLocation();
	FVector ForwardVector = AttackTracePos->GetForwardVector();
	FVector End = Start + (ForwardVector * 100.0f);

	// 设置攻击判定参数
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
		// 处理攻击命中逻辑
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

	// 减少角色生命值
	Health -= DamageAmount;


	// 播放受伤声音
	UGameplayStatics::PlaySoundAtLocation(this, DamageSound, GetActorLocation());

	// 如果生命值小于等于0，处理角色死亡逻辑
	if (Health <= 0)
	{
		Health = 0;

		// 禁用输入
		APlayerController* PlayerController = Cast<APlayerController>(GetController());
		if (PlayerController)
		{
			DisableInput(PlayerController);
		}

		

		// 启用模拟物理
		//GetMesh()->SetSimulatePhysics(true);

		GetMesh()->SetSimulatePhysics(true); // 设置模拟物理
		GetMesh()->SetCollisionProfileName(TEXT("Custom")); // 设置碰撞预设为自定义
		GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics); // 启用查询和物理碰撞
		GetMesh()->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block); // 设置所有碰撞通道的默认响应
		GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
		GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);

		bCanAttack = false;
	}
	else
	{
		// 执行其他受伤逻辑
		FVector LaunchVelocity = GetActorForwardVector() * -200.0f + GetActorUpVector() * 100.0f;
		LaunchCharacter(LaunchVelocity, true, true);
	}

	return DamageAmount;
}

