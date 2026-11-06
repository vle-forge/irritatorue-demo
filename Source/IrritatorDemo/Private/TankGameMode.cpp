// Copyright (c) 2026 INRAE Distributed under the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include "TankGameMode.h"

#include "TankActor.h"

#include "Camera/CameraActor.h"
#include "Components/LightComponent.h"
#include "Components/SkyLightComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/DirectionalLight.h"
#include "Engine/SkyLight.h"
#include "Engine/StaticMesh.h"
#include "Engine/StaticMeshActor.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"

ATankGameMode::ATankGameMode()
{
    // No pawn: the view is the fixed camera.
    DefaultPawnClass = nullptr;
}

void ATankGameMode::StartPlay()
{
    Super::StartPlay();

    UWorld* World = GetWorld();

    World->SpawnActor<ATankActor>(FVector::ZeroVector, FRotator::ZeroRotator);

    if (ADirectionalLight* Sun = World->SpawnActor<ADirectionalLight>(FVector(0.0, 0.0, 500.0), FRotator(-50.0, 30.0, 0.0)))
        Sun->GetLightComponent()->SetIntensity(5.f);

    if (ASkyLight* Sky = World->SpawnActor<ASkyLight>(FVector(0.0, 0.0, 500.0), FRotator::ZeroRotator))
        Sky->GetLightComponent()->SetIntensity(1.f);

    if (UStaticMesh* Plane = LoadObject<UStaticMesh>(nullptr, TEXT("/Engine/BasicShapes/Plane.Plane"))) {
        FActorSpawnParameters Params;
        if (AStaticMeshActor* Floor = World->SpawnActor<AStaticMeshActor>(FVector::ZeroVector, FRotator::ZeroRotator, Params)) {
            Floor->GetStaticMeshComponent()->SetMobility(EComponentMobility::Movable);
            Floor->GetStaticMeshComponent()->SetStaticMesh(Plane);
            Floor->SetActorScale3D(FVector(10.0, 10.0, 1.0));
        }
    }

    ACameraActor* Camera = World->SpawnActor<ACameraActor>(FVector(-700.0, 0.0, 260.0), FRotator(-12.0, 0.0, 0.0));

    if (APlayerController* PC = World->GetFirstPlayerController(); PC and Camera)
        PC->SetViewTarget(Camera);
}
