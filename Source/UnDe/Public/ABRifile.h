// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UnDe.h"
#include "GameFramework/Actor.h"
#include "ABRifile.generated.h"

UCLASS()
class UNDE_API AABRifile : public AActor
{
	GENERATED_BODY()
	
public:	
	AABRifile();

protected:
	virtual void BeginPlay() override;

public:
	UPROPERTY(VisibleAnywhere, Category = Weapon)
	USkeletalMeshComponent* SK_AKRifle;

};
