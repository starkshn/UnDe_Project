// Fill out your copyright notice in the Description page of Project Settings.


#include "ABMyCharacter.h"
#include "ABMyAnim.h"

// Sets default values
AABMyCharacter::AABMyCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = GetCapsuleComponent();

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SPRINGARM"));
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("CAMERA"));


	SpringArm->SetupAttachment(GetCapsuleComponent());
	Camera->SetupAttachment(SpringArm);

	GetMesh()->SetRelativeLocationAndRotation(FVector(0.f, 0.f, -88.f), FRotator(0.f, -90.f, 0.f));
	SpringArm->TargetArmLength = 400.f;
	SpringArm->SetRelativeRotation(FRotator(-15.f, 0.f, 0.f));

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> MY_Mesh(TEXT("SkeletalMesh'/Game/InfinityBladeWarriors/Character/CompleteCharacters/sk_CharM_Base.sk_CharM_Base'"));
	if (MY_Mesh.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(MY_Mesh.Object);
	}

	static ConstructorHelpers::FClassFinder<UAnimInstance> MY_Anim(TEXT("AnimBlueprint'/Game/Blueprints/BP_MyCharacter.BP_MyCharacter_C'"));
	if (MY_Anim.Succeeded())
	{
		GetMesh()->SetAnimInstanceClass(MY_Anim.Class);
	}

	// Set
	SetControlMode(ControlMode::DAIBLO);

	ArmLengthSpeed = 3.f;
	ArmRotationSpeed = 10.f;
	GetCharacterMovement()->JumpZVelocity = 800.f;

	IsAttacking_Sword = false;
	MaxCombo_Sword = 3;
	AttackEndComboState_Sword();
}

// Called when the game starts or when spawned
void AABMyCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void AABMyCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	
	MyAnim = Cast<UABMyAnim>(GetMesh()->GetAnimInstance());
	ABCHECK(nullptr != MyAnim);

	MyAnim->OnMontageEnded.AddDynamic(this, &AABMyCharacter::OnAttackSwordMontageEnded);

	MyAnim->OnNextAttackCheck_Sword.AddLambda
	(
		[this]() -> void
		{
			CanNextCombo_Sword = false;
			
			if (IsComboInputOn_Sword)
			{
				AttackStartState_Sword();
				MyAnim->JumpToAttackMontageSection_Sword(CurrentCombo_Sword);
			}
		}
	);
}

void AABMyCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	ABLOG_S(Warning);
}

// Called every frame
void AABMyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	SpringArm->TargetArmLength = FMath::FInterpTo(SpringArm->TargetArmLength, ArmLengthTo, DeltaTime, ArmLengthSpeed);

	switch (CurrentControlMode)
	{
		case ControlMode::DAIBLO:
		{
			FRotator r = FMath::RInterpTo(SpringArm->GetRelativeRotation(), ArmRotationTo, DeltaTime, ArmRotationSpeed);
			SpringArm->SetRelativeRotation(r);
		}
		break;

	}

	switch (CurrentControlMode)
	{
		case ControlMode::DAIBLO:
		{
			if (DirectionToMove.SizeSquared() > 0.f)
			{
				GetController()->SetControlRotation(FRotationMatrix::MakeFromX(DirectionToMove).Rotator());
				{
					AddMovementInput(DirectionToMove);
				}
			}
		}
		break;
		case ControlMode::GTA:
		{
			
		}
		break;
	}
}

// Called to bind functionality to input
void AABMyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction(TEXT("ViewChange"), EInputEvent::IE_Pressed, this, &AABMyCharacter::ViewChange);

	PlayerInputComponent->BindAxis(TEXT("UpDown"), this, &AABMyCharacter::UpDown);
	PlayerInputComponent->BindAxis(TEXT("LeftRight"), this, &AABMyCharacter::LeftRight);
	PlayerInputComponent->BindAxis(TEXT("LookUp"), this, &AABMyCharacter::LookUp);
	PlayerInputComponent->BindAxis(TEXT("Turn"), this, &AABMyCharacter::Turn);

	PlayerInputComponent->BindAction(TEXT("Jump"), EInputEvent::IE_Pressed, this, &AABMyCharacter::Jump);
	PlayerInputComponent->BindAction(TEXT("AttackDefault"), EInputEvent::IE_Pressed, this, &AABMyCharacter::AttackSword);
}

void AABMyCharacter::UpDown(float NewAxisValue)
{
	switch (CurrentControlMode)
	{
		case ControlMode::GTA:
		{
			AddMovementInput(FRotationMatrix(FRotator(0.f, GetControlRotation().Yaw, 0.f)).GetUnitAxis(EAxis::X), NewAxisValue);
		}
		break;
		case ControlMode::DAIBLO:
		{
			DirectionToMove.X = NewAxisValue;
		}
		break;
	}
}

void AABMyCharacter::LeftRight(float NewAxisValue)
{
	switch (CurrentControlMode)
	{
		case ControlMode::GTA:
		{
			AddMovementInput(FRotationMatrix(FRotator(0.f, GetControlRotation().Yaw, 0.f)).GetUnitAxis(EAxis::Y), NewAxisValue);
		}
		break;
		case ControlMode::DAIBLO:
		{
			DirectionToMove.Y = NewAxisValue;
		}
		break;
	}
}

void AABMyCharacter::LookUp(float NewAxisValue)
{
	switch (CurrentControlMode)
	{
		case ControlMode::GTA:
		{
			AddControllerPitchInput(NewAxisValue);
		}
		break;
	}
}

void AABMyCharacter::Turn(float NewAxisValue)
{
	switch (CurrentControlMode)
	{
		case ControlMode::GTA:
		{
			AddControllerYawInput(NewAxisValue);
		}
		break;
	}
}

void AABMyCharacter::ViewChange()
{
	switch (CurrentControlMode)
	{
		case ControlMode::GTA:
		{
			GetController()->SetControlRotation(GetActorRotation());
			SetControlMode(ControlMode::DAIBLO);
		}
		break;
		case ControlMode::DAIBLO:
		{
			GetController()->SetControlRotation(SpringArm->GetRelativeRotation());
			SetControlMode(ControlMode::GTA);
		}
		break;	
	}
}

void AABMyCharacter::AttackSword()
{
	if (IsAttacking_Sword)
	{
		ABCHECK(FMath::IsWithinInclusive<int32>(CurrentCombo_Sword, 1, MaxCombo_Sword));
		if (CanNextCombo_Sword) 
			IsComboInputOn_Sword = true;
	}
	else
	{
		// ABCHECK(CurrentCombo == 0);
		AttackStartState_Sword();
		MyAnim->PlayAttackMontage_Sword();
		MyAnim->JumpToAttackMontageSection_Sword(CurrentCombo_Sword);
		IsAttacking_Sword = true;
	}
}

void AABMyCharacter::AttackStartState_Sword()
{
	CanNextCombo_Sword = true;
	IsComboInputOn_Sword = false;
	CurrentCombo_Sword = FMath::Clamp<int32>(CurrentCombo_Sword + 1, 1, MaxCombo_Sword);
}

void AABMyCharacter::AttackEndComboState_Sword()
{
	IsComboInputOn_Sword = false;
	CanNextCombo_Sword = false;
	CurrentCombo_Sword = 0;
}

void AABMyCharacter::OnAttackSwordMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	ABCHECK(IsAttacking_Sword);
	ABCHECK(CurrentCombo_Sword > 0);
	IsAttacking_Sword = false;
	AttackEndComboState_Sword();
}

void AABMyCharacter::SetControlMode(ControlMode ControlMode)
{
	CurrentControlMode = ControlMode;

	switch (CurrentControlMode)
	{
		case ControlMode::DAIBLO:
		{
			ArmLengthTo = 800.f;
			ArmRotationTo = FRotator(-45.f, 0.f, 0.f);
			// SpringArm->TargetArmLength = 800.f;
			// SpringArm->SetRelativeRotation(FRotator(-45.0f, 0.f, 0.f));
			SpringArm->bUsePawnControlRotation = false;
			SpringArm->bInheritPitch = false;
			SpringArm->bInheritRoll = false;
			SpringArm->bInheritYaw = false;
			SpringArm->bDoCollisionTest = false;
			bUseControllerRotationYaw = false;
			GetCharacterMovement()->bOrientRotationToMovement = false;
			GetCharacterMovement()->bUseControllerDesiredRotation = true;
			GetCharacterMovement()->RotationRate = FRotator(0.f, 720.f, 0.f);
		}
		break;
		case ControlMode::GTA:
		{
			ArmLengthTo = 450.f;
			// SpringArm->TargetArmLength = 450.f;
			// SpringArm->SetRelativeRotation(FRotator::ZeroRotator);
			SpringArm->bUsePawnControlRotation = true;
			SpringArm->bInheritPitch = true;
			SpringArm->bInheritRoll = true;
			SpringArm->bInheritYaw = true;
			SpringArm->bDoCollisionTest = true;
			bUseControllerRotationYaw = false;
			GetCharacterMovement()->bOrientRotationToMovement = true;
			GetCharacterMovement()->bUseControllerDesiredRotation = false;
			GetCharacterMovement()->RotationRate = FRotator(0.f, 720.f, 0.f);
		}
		break;
	}
}

