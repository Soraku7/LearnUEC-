// Fill out your copyright notice in the Description page of Project Settings.


#include "FPSExtractionZone.h"

#include "FPSCharacter.h"
#include "FPSGameMode.h"
#include "Components/DecalComponent.h"


// Sets default values
AFPSExtractionZone::AFPSExtractionZone()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	OverlapComp = CreateDefaultSubobject<UBoxComponent>(TEXT("OverlapComp"));
	OverlapComp -> SetCollisionEnabled(ECollisionEnabled:: QueryOnly);
	OverlapComp -> SetCollisionResponseToChannels(ECR_Ignore);
	OverlapComp -> SetCollisionResponseToChannel(ECC_Pawn , ECR_Overlap);
	OverlapComp -> SetBoxExtent(FVector(200.0f));
	RootComponent =	OverlapComp;
	
	OverlapComp -> SetHiddenInGame(false);
	OverlapComp -> OnComponentBeginOverlap.AddDynamic(this , &AFPSExtractionZone::HandelOverlap);

	DecalComp = CreateDefaultSubobject<UDecalComponent>(TEXT("DecalComp"));
	DecalComp -> DecalSize = FVector(200.0f,200.0f,200.0f);
	DecalComp -> SetupAttachment(RootComponent); 
}

// Called when the game starts or when spawned
void AFPSExtractionZone::BeginPlay()
{
	Super::BeginPlay();
	
}

void AFPSExtractionZone::HandelOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UE_LOG(LogTemp , Log , TEXT("Overlaped with extraction zone!"));

	AFPSCharacter* MyPawn = Cast<AFPSCharacter>(OtherActor);
	if(MyPawn && MyPawn -> bIsCarryingObjective)
	{
		AFPSGameMode* GM = Cast<AFPSGameMode>(GetWorld() -> GetAuthGameMode());
		if(GM)
		{
			GM -> CompleteMission(MyPawn);
		}
	}
}



