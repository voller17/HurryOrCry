// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
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

//Variables
public:
	UPROPERTY(BlueprintReadWrite,EditInstanceOnly)
	EPointType PointType = EPointType::FinishPoint;	
};
