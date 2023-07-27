// Fill out your copyright notice in the Description page of Project Settings.


#include "FPSObjectiveActor.h"

#include "FPSCharacter.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AFPSObjectiveActor::AFPSObjectiveActor()
{

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	MeshComp -> SetCollisionEnabled(ECollisionEnabled::NoCollision);
	RootComponent = MeshComp;
	
	SphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	//设置碰撞组件仅查询
	SphereComp -> SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	
	SphereComp -> SetCollisionResponseToChannels(ECR_Ignore);
	SphereComp -> SetCollisionResponseToChannel(ECC_Pawn , ECR_Overlap);
	
	SphereComp -> SetupAttachment(MeshComp);

}

// Called when the game starts or when spawned
void AFPSObjectiveActor::BeginPlay()
{
	Super::BeginPlay();

	PlayEffects();
	
}


/**
 * @brief 碰撞监听事件
 * @param OtherActor 
 */
void AFPSObjectiveActor::NotifyActorBeginOverlap(AActor* OtherActor)
{
	//调用基类函数
	Super::NotifyActorBeginOverlap(OtherActor);

	PlayEffects();

	//将OtherActor强制转换成AFPSCharacter类型，也是对MyCharacter进行初始化
	AFPSCharacter* MyCharacter = Cast<AFPSCharacter>(OtherActor);
	if(MyCharacter)
	{
		MyCharacter -> bIsCarryingObjective = true;
		Destroy();
	}
}

void AFPSObjectiveActor::PlayEffects()
{
	//初始化粒子效果
	UGameplayStatics::SpawnEmitterAtLocation(this , PickupFX , GetActorLocation());
}


