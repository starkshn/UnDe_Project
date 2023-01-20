// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UnDe.h"
#include "GameFramework/Character.h"
#include "ABMyCharacter.generated.h"

DECLARE_DELEGATE_OneParam(FOnPressedUpDown, bool);
DECLARE_DELEGATE_OneParam(FOnPressedLeftRight, bool);
DECLARE_DELEGATE_OneParam(FOnAimingEvent, bool);
DECLARE_DELEGATE_OneParam(FOnWeaponChangedEvent, int32);

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
		Sword = 1,
		Rifle = 2,
		Hand = 3,
	};

public:
	FOnPressedUpDown		UpDownEvent;
	FOnPressedLeftRight		LeftRightEvent;
	FOnAimingEvent			AimingEvent;
	FOnWeaponChangedEvent	WeaponChangedEvent;

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
	USkeletalMeshComponent* SK_Rifile;

	UPROPERTY(VisibleAnywhere, Category = WEAPON)
	USkeletalMeshComponent* SK_Sword;

	UPROPERTY(VisibleAnywhere, Category = WEAPON)
	class AABRifile* CurRifile;

private:
	void ViewChange();

	void UpDown(float NewAxisValue);
	void LeftRight(float NewAxisValue);
	void LookUp(float NewAxisValue);
	void Turn(float NewAxisValue);
	void Jump();

	void UpDownP();
	void UpDownR();
	void LeftRightP();
	void LeftRightR();
	void PressedAiming();
	void ReleasedAiming();

	void AttackSword();
	void AttackStartComboState_Sword();
	void AttackEndComboState_Sword();

	void ChangeToHand();
	void ChangeToSword();
	void ChangeToRifile();

	void EquipRifile();
	void ReleaseRifile();

private:
	UFUNCTION()
	void OnAttackSwordMontageEnded(UAnimMontage* Montage, bool bInterrupted);

private:
	void SetControlMode(ControlMode ControlMode);
	void SetWeapon(WeaponMode WeaponMode) { CurrentWeapon = WeaponMode; }
	void SetCrouch();

public:
	bool GetCrouch() { return (bool)IsCrouch; }

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

	// Sockets
	FName		BackRifileSocket = (TEXT("Back_Rifile_Socket"));
	FName		HandRightSocket = (TEXT("hand_rSocket"));

};
