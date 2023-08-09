// Fill out your copyright notice in the Description page of Project Settings.


#include "FPSLaunchPad.h"

#include "Kismet/GameplayStatics.h"
#include "Tasks/Task.h"


// Sets default values
AFPSLaunchPad::AFPSLaunchPad()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	OverlapComp = CreateDefaultSubobject<UBoxComponent>(TEXT("OverlapComp"));
	OverlapComp ->SetBoxExtent(FVector(75 , 75 , 75));
	RootComponent = OverlapComp;

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	MeshComp -> SetupAttachment(RootComponent);

	OverlapComp -> OnComponentBeginOverlap.AddDynamic(this , &AFPSLaunchPad::OverlapLaunchPad);

	LaunchStrength = 1500.0;
	LaunchPitchAngle = 35.0;
}

// Called when the game starts or when spawned
void AFPSLaunchPad::BeginPlay()
{
	Super::BeginPlay();
	
}

void AFPSLaunchPad::OverlapLaunchPad(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//获取物体面对方向
	FRotator LaunchDirection = GetActorRotation();
	LaunchDirection.Pitch += LaunchPitchAngle;
	FVector LaunchVelocity = LaunchDirection.Vector() * LaunchStrength;

	ACharacter* OtherCharacter = Cast<ACharacter>(OtherActor);
	if(OtherCharacter)
	{
		OtherCharacter -> LaunchCharacter(LaunchVelocity , true , true);
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld() , ActivateLaunchPadEffect , GetActorLocation());
	}
	//判断是否模拟物理 发射物体
	else if(OtherComp && OtherComp -> IsSimulatingPhysics())
	{
		//添加物体加速
		OtherComp -> AddImpulse(LaunchVelocity , NAME_None , true);
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld() , ActivateLaunchPadEffect , GetActorLocation());
	}
}

// Called every frame
void AFPSLaunchPad::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

