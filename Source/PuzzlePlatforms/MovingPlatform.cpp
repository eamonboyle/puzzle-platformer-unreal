// Fill out your copyright notice in the Description page of Project Settings.


#include "MovingPlatform.h"

AMovingPlatform::AMovingPlatform()
{
    PrimaryActorTick.bCanEverTick = true;

    SetMobility(EComponentMobility::Movable);
}

void AMovingPlatform::BeginPlay()
{
    Super::BeginPlay();

    if (HasAuthority())
    {
        SetReplicates(true);
        SetReplicateMovement(true);
    }

    GlobalStartLocation = GetActorLocation();
    GlobalTargetLocation = GetTransform().TransformPosition(TargetLocation);
}

void AMovingPlatform::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (ActiveTriggers > 0)
    {
        if (HasAuthority())
        {
            FVector Location = GetActorLocation();
            const float JourneyLength = (GlobalTargetLocation - GlobalStartLocation).Size();
            const float JourneyTravelled = (Location - GlobalStartLocation).Size();

            if (JourneyTravelled >= JourneyLength)
            {
                const FVector Swap = GlobalStartLocation;
                GlobalStartLocation = GlobalTargetLocation;
                GlobalTargetLocation = Swap;
            }
            const FVector Direction = (GlobalTargetLocation - GlobalStartLocation).GetSafeNormal();
            Location += Speed * DeltaTime * Direction;

            SetActorLocation(Location);
        }
    }
}

void AMovingPlatform::AddActiveTrigger()
{
    ActiveTriggers++;
}

void AMovingPlatform::RemoveActiveTrigger()
{
    if (ActiveTriggers > 0)
    {
        ActiveTriggers--;
    }
}
