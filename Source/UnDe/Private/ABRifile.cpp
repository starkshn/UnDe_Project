// Fill out your copyright notice in the Description page of Project Settings.


#include "ABRifile.h"

// Sets default values
AABRifile::AABRifile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

    SK_AKRifle = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WEAPON"));
    RootComponent = SK_AKRifle;

    static ConstructorHelpers::FObjectFinder<USkeletalMesh> SK_WEAPON(TEXT("SkeletalMesh'/Game/FPS_Weapon_Bundle/Weapons/Meshes/Ka47/SK_KA47.SK_KA47'"));
    if (SK_WEAPON.Succeeded())
    {
        SK_AKRifle->SetSkeletalMesh(SK_WEAPON.Object);
    }
    SK_AKRifle->SetCollisionProfileName(TEXT("NoCollision"));
}

// Called when the game starts or when spawned
void AABRifile::BeginPlay()
{
	Super::BeginPlay();
}


