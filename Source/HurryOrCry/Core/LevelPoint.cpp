#include "LevelPoint.h"
#include "BaseSessionGameState.h"


// Sets default values
ALevelPoint::ALevelPoint()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = false;
}

// Called when the game starts or when spawned
void ALevelPoint::BeginPlay()
{
	Super::BeginPlay();
	if (GetNetMode() != NM_Client)
	{
		
	}
	if (ABaseSessionGameState* BaseSessionGameState = GetWorld()->GetGameState<ABaseSessionGameState>())
	{
		switch (PointType)
		{
			case EPointType::FinishPoint:
				BaseSessionGameState->FinishLevelPoint = this;
			break;
			case EPointType::StartPoint:
				BaseSessionGameState->StartLevelPoint = this;
			break;
		}
	}
}

