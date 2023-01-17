// Fill out your copyright notice in the Description page of Project Settings.


#include "ABMyAnim.h"
#include "ABMyCharacter.h"

UABMyAnim::UABMyAnim()
{
	static ConstructorHelpers::FObjectFinder<UAnimMontage> ATTACK_DEFAULT_M(TEXT("AnimMontage'/Game/Animations/MT_MyCharacterDefault.MT_MyCharacterDefault'"));
	if (ATTACK_DEFAULT_M.Succeeded())
	{
		AttackMontage_Sword = ATTACK_DEFAULT_M.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> ATTACK_GUN_M(TEXT("AnimMontage'/Game/Animations/MT_MyCharacterGun.MT_MyCharacterGun'"));
	if (ATTACK_DEFAULT_M.Succeeded())
	{
		AttackMontage_Gun = ATTACK_GUN_M.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> EQUIP_RIFILE(TEXT("AnimMontage'/Game/Animations/MT_MyRifileEquip.MT_MyRifileEquip'"));
	if (EQUIP_RIFILE.Succeeded())
	{
		EquipCheckMontage_Rifile = EQUIP_RIFILE.Object;
	}

	CurrentPawnSpeed = 0.f;
	IsInAir = false;

	CurrentWeapon = 0;
}

void UABMyAnim::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	auto Pawn = TryGetPawnOwner();
	if (::IsValid(Pawn))
	{
		CurrentPawnSpeed = Pawn->GetVelocity().Size();

		auto Character = Cast<AABMyCharacter>(Pawn);
		if (Character)
		{
			IsInAir = Character->GetMovementComponent()->IsFalling();
		}
	}
}

void UABMyAnim::PlayAttackMontage_Sword()
{	
	if (CurrentWeapon == 1)
	{
		Montage_Play(AttackMontage_Sword, 1.0f);
	}
}

void UABMyAnim::JumpToAttackMontageSection_Sword(int32 NewSection)
{
	ABCHECK(Montage_IsPlaying(AttackMontage_Sword));
	ABLOG(Warning, TEXT("%d"), NewSection);
	Montage_JumpToSection(GetAttackMontageSectionName(NewSection), AttackMontage_Sword);
}

void UABMyAnim::AnimNotify_AttackHitCheck_Sword()
{
	OnAttackHitCheck_Sword.Broadcast();
}

void UABMyAnim::AnimNotify_NextAttackCheck_Sword()
{
	OnNextAttackCheck_Sword.Broadcast();
}

void UABMyAnim::AnimNotify_EquipFinish()
{
	ABLOG(Warning, TEXT("AnimNotify_EquipFinish"));
	OnEquipFinishCheck_Rifile.Broadcast();
}

void UABMyAnim::PlayEquipMontage_Rifile()
{
	ABLOG(Warning, TEXT("PlayEquipMontage_Rifile"));
	if (CurrentWeapon == 2)
	{
		Montage_Play(EquipCheckMontage_Rifile, 1.0f);
	}
}

FName UABMyAnim::GetAttackMontageSectionName(int32 Section)
{
	ABCHECK(FMath::IsWithinInclusive<int32>(Section, 1, 3), NAME_None);
	return FName(*FString::Printf(TEXT("Attack%d"), Section));
}
