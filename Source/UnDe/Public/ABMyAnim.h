// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UnDe.h"
#include "Animation/AnimInstance.h"
#include "ABMyAnim.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnNextAttackCheckDelegate_Sword);
DECLARE_MULTICAST_DELEGATE(FOnAttackHitCheckDelegate_Sword);


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
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;


public:
	void PlayAttackMontage_Sword();
	void JumpToAttackMontageSection_Sword(int32 NewSection);

private:
	UFUNCTION()
	void AnimNotify_AttackHitCheck_Sword();

	UFUNCTION()
	void AnimNotify_NextAttackCheck_Sword();

public:
	FOnNextAttackCheckDelegate_Sword OnNextAttackCheck_Sword;
	FOnAttackHitCheckDelegate_Sword OnAttackHitCheck_Sword;

private:
	FName GetAttackMontageSectionName(int32 Section);

private:
	UPROPERTY(EditAnywhere, BlueprintreadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
	float CurrentPawnSpeed;

	UPROPERTY(EditAnywhere, BlueprintreadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
	bool IsInAir;

	UPROPERTY(EditAnywhere, BlueprintreadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	UAnimMontage* AttackMontage_Sword;

	UPROPERTY(EditAnywhere, BlueprintreadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	UAnimMontage* AttackMontage_Gun;
	
};
