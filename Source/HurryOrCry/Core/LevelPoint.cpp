#include "LevelPoint.h"
#include "BaseSessionGameState.h"

// Sets default values
ALevelPoint::ALevelPoint()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = false;

	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	BoxComponent->SetupAttachment(RootComponent);

	// Set BoxComponent as the root component
	RootComponent = BoxComponent;

	// Register overlap event
}

// Called when the game starts or when spawned
void ALevelPoint::BeginPlay()
{
	Super::BeginPlay();
	if (ABaseSessionGameState* BaseSessionGameState = GetWorld()->GetGameState<ABaseSessionGameState>())
	{
		switch (PointType)
		{
			case EPointType::FinishPoint:
				BaseSessionGameState->FinishLevelPoint = this;
				BoxComponent->OnComponentBeginOverlap.AddDynamic(this, &ALevelPoint::OnOverlapBegin);
			break;
			case EPointType::StartPoint:
				BaseSessionGameState->StartLevelPoint = this;
			break;
		}
	}
}

void ALevelPoint::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (GetNetMode() != NM_Client)
	{
		if (OtherActor && (OtherActor != this) && OtherComp)
		{
			if (const ABaseCharacter* Character = Cast<ABaseCharacter>(OtherActor))
			{
				if (ABaseSessionGameState* GameState = GetWorld()->GetGameState<ABaseSessionGameState>())
				{
					if(ABaseSessionPlayerState* BaseSessionPlayerState = Cast<ABaseSessionPlayerState>(Character->GetPlayerState()))
					{
						GameState->SetWinnerPlayer(BaseSessionPlayerState);
						UE_LOG(LogTemp, Warning, TEXT("ALevelPoint SetWinnerPlayer : %s"), *BaseSessionPlayerState->GetPlayerName());
					}					
				}
			}
		}
	}	
}

