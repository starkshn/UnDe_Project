// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UnDe.h"
#include "GameFramework/Character.h"
#include "ABMyCharacter.generated.h"

UCLASS()
class UNDE_API AABMyCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AABMyCharacter();

protected:
	virtual void BeginPlay() override;
	virtual void PostInitializeComponents() override;
	virtual void PossessedBy(AController* NewController) override;

	enum class ControlMode
	{
		GTA,
		DAIBLO,
	};

	enum class WeaponMode
	{
		Hand,
		Sword,
		Rifile,
	};
	
public:	
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(VisibleAnywhere, Category = Camera)
	USpringArmComponent* SpringArm;
	
	UPROPERTY(VisibleAnywhere, Category = Camera)
	UCameraComponent* Camera;

	UPROPERTY()
	class UABMyAnim* MyAnim;

public:
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	bool IsAttacking_Sword;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowprivateAccess = true))
	bool CanNextCombo_Sword;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowprivateAccess = true))
	bool IsComboInputOn_Sword;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowprivateAccess = true))
	int32 CurrentCombo_Sword;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowprivateAccess = true))
	int32 MaxCombo_Sword;

	UPROPERTY(VisibleAnywhere, Category = WEAPON)
	USkeletalMeshComponent* Weapon;

	UPROPERTY(VisibleAnywhere, Category = WEAPON)
	USkeletalMeshComponent* WeaponHand;
private:
	void ViewChange();

	void UpDown(float NewAxisValue);
	void LeftRight(float NewAxisValue);
	void LookUp(float NewAxisValue);
	void Turn(float NewAxisValue);
	void Crouch();

	void AttackSword();
	void AttackStartComboState_Sword();
	void AttackEndComboState_Sword();

	void ChangeToHand();
	void ChangeToSword();
	void ChangeToRifile();

private:
	UFUNCTION()
	void OnAttackSwordMontageEnded(UAnimMontage* Montage, bool bInterrupted);

protected:
	void SetControlMode(ControlMode ControlMode);
	void SetWeapon(WeaponMode WeaponMode) { CurrentWeapon = WeaponMode; }

public:
	WeaponMode GetCurrentWeapon() { return CurrentWeapon; }

private:
	ControlMode CurrentControlMode = ControlMode::GTA;
	FVector		DirectionToMove = FVector::ZeroVector;

	WeaponMode	CurrentWeapon = WeaponMode::Hand;

	float		ArmLengthTo = 0.0f;
	FRotator	ArmRotationTo = FRotator::ZeroRotator;
	float		ArmLengthSpeed = 0.0f;
	float		ArmRotationSpeed = 0.0f;

	int32		IsCrouch = 0;
};
