// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Actor.h"
#include "LevelPoint.generated.h"


UENUM(BlueprintType)
enum class EPointType : uint8
{
	StartPoint,
	FinishPoint
};

UCLASS()
class HURRYORCRY_API ALevelPoint : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ALevelPoint();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	
private:
	// Overlap function
	UFUNCTION()
	void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor,
						class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

//Variables
public:
	UPROPERTY(BlueprintReadWrite,EditInstanceOnly)
	EPointType PointType = EPointType::FinishPoint;

private:
	// Box Component for collision
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UBoxComponent* BoxComponent;

};
