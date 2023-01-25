// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UnDe.h"
#include "Animation/AnimInstance.h"
#include "ABMyAnim.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnNextAttackCheckDelegate_Sword);
DECLARE_MULTICAST_DELEGATE(FOnAttackHitCheckDelegate_Sword);
DECLARE_MULTICAST_DELEGATE(FOnEquipFinishDelegate_Rifile);
DECLARE_MULTICAST_DELEGATE(FOnReleaseFinishDelegate_Rifile);

/**
 * 
 */
UCLASS()
class UNDE_API UABMyAnim : public UAnimInstance
{
	GENERATED_BODY()

public:
	UABMyAnim();

public:
	virtual void NativeBeginPlay() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

public:
	void PlayAttackMontage_Sword();
	void JumpToAttackMontageSection_Sword(int32 NewSection);


private:
	UFUNCTION()
	void AnimNotify_AttackHitCheck_Sword();

	UFUNCTION()
	void AnimNotify_NextAttackCheck_Sword();

	UFUNCTION()
	void AnimNotify_EquipRifle();

	UFUNCTION()
	void AnimNotify_ReleaseRifle();

public:
	FOnNextAttackCheckDelegate_Sword	OnNextAttackCheck_Sword;
	FOnAttackHitCheckDelegate_Sword		OnAttackHitCheck_Sword;
	FOnEquipFinishDelegate_Rifile		OnEquipFinishCheck_Rifile;
	FOnReleaseFinishDelegate_Rifile		OnReleaseFinish_Rifile;

private:
	FName GetAttackMontageSectionName_Sword(int32 Section);

public:
	void SetCurrentWeapon(int32 Weapon) { ABLOG(Warning, TEXT("Weapon : %d"), Weapon); CurrentWeapon = Weapon; }
	void SetCrouch(int32 Crouch) { IsCrouch = Crouch; }
	void SetUpDownEvent(bool Event) 
	{ 
		UpDownEvent = Event;
	}
	void SetLeftRightEvent(bool Event) 
	{ 
		LeftRightEvent = Event; 
	}

private:
	UPROPERTY(EditAnywhere, BlueprintreadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
	float CurrentPawnSpeed;

	UPROPERTY(EditAnywhere, BlueprintreadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
	bool IsInAir;

	UPROPERTY(EditAnywhere, BlueprintreadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
	int32 CurrentWeapon;

	UPROPERTY(EditAnywhere, BlueprintreadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
	int32 IsCrouch;

	UPROPERTY(EditAnywhere, BlueprintreadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
	bool UpDownEvent;

	UPROPERTY(EditAnywhere, BlueprintreadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))

	bool LeftRightEvent;

	UPROPERTY(EditAnywhere, BlueprintreadWrite, Category = Pawn, Meta = (AllowPrivateAccess = true))

	bool IsAiming;

	UPROPERTY(EditAnywhere, BlueprintreadWrite, Category = Pawn, Meta = (AllowPrivateAccess = true))

	int32 VaultState;


	// ###############################

	UPROPERTY(EditAnywhere, BlueprintreadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	UAnimMontage* AttackMontage_Sword;

	UPROPERTY(EditAnywhere, BlueprintreadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	UAnimMontage* AttackMontage_Gun;

	UPROPERTY(EditAnywhere, BlueprintreadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	UAnimMontage* EquipCheckMontage_Rifile;

private:
	class AABMyCharacter* MyCharacater;
	int32 ForUpDown = 0;
	int32 ForLeftRight = 0;
};
