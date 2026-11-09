// Copyright (c) 2026 INRAE Distributed under the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include "TankGameMode.h"

#include "TankActor.h"
#include "TankHUD.h"

#include "Camera/CameraActor.h"
#include "Components/DirectionalLightComponent.h"
#include "Components/SkyAtmosphereComponent.h"
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
    HUDClass         = ATankHUD::StaticClass();
}

void ATankGameMode::StartPlay()
{
    Super::StartPlay();

    UWorld* World = GetWorld();

    World->SpawnActor<ATankActor>(FVector::ZeroVector, FRotator::ZeroRotator);

    // The lights of a map built at run time: movable, nothing is baked.
    World->SpawnActor<ASkyAtmosphere>(FVector::ZeroVector, FRotator::ZeroRotator);

    if (ADirectionalLight* Sun = World->SpawnActor<ADirectionalLight>(FVector(0.0, 0.0, 500.0), FRotator(-40.0, 140.0, 0.0))) {
        auto* Light = CastChecked<UDirectionalLightComponent>(Sun->GetLightComponent());
        Light->SetMobility(EComponentMobility::Movable);
        Light->SetIntensity(8.f);
        Light->SetAtmosphereSunLight(true);
    }

    if (ASkyLight* Sky = World->SpawnActor<ASkyLight>(FVector(0.0, 0.0, 500.0), FRotator::ZeroRotator)) {
        USkyLightComponent* Light = Sky->GetLightComponent();
        Light->SetMobility(EComponentMobility::Movable);
        Light->bRealTimeCapture = true;
        Light->RecaptureSky();
    }

    if (UStaticMesh* Plane = LoadObject<UStaticMesh>(nullptr, TEXT("/Engine/BasicShapes/Plane.Plane"))) {
        if (AStaticMeshActor* Floor = World->SpawnActor<AStaticMeshActor>(FVector::ZeroVector, FRotator::ZeroRotator)) {
            Floor->GetStaticMeshComponent()->SetMobility(EComponentMobility::Movable);
            Floor->GetStaticMeshComponent()->SetStaticMesh(Plane);
            Floor->SetActorScale3D(FVector(400.0, 400.0, 1.0)); // to the horizon
        }
    }

    UseCamera(World->GetFirstPlayerController());
}

void ATankGameMode::PostLogin(APlayerController* NewPlayer)
{
    Super::PostLogin(NewPlayer);

    // A player that arrives after StartPlay(): the scene is there.
    if (HasMatchStarted())
        UseCamera(NewPlayer);
}

void ATankGameMode::UseCamera(APlayerController* PC)
{
    if (not PC)
        return;

    if (not Camera)
        Camera = GetWorld()->SpawnActor<ACameraActor>(FVector(-900.0, 0.0, 280.0), FRotator(-10.0, 0.0, 0.0));

    // Otherwise the controller takes its own camera back (no pawn to view).
    PC->bAutoManageActiveCameraTarget = false;
    PC->SetViewTarget(Camera);
}
