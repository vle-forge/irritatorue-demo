// Copyright (c) 2026 INRAE Distributed under the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include "TankActor.h"

#include "IrritatorObserverComponent.h"
#include "IrritatorSubsystem.h"

#include "Components/InputComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/StaticMesh.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"
#include "InputCoreTypes.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Materials/Material.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "UObject/ConstructorHelpers.h"

namespace {

constexpr double Threshold = 5.0; // the level of the overflow in tank.pirt

const FLinearColor Normal(0.1f, 0.6f, 0.2f);
const FLinearColor Alarm(0.9f, 0.1f, 0.05f);
const FLinearColor MarkerColor(0.9f, 0.7f, 0.1f);

// The cube of 1 m is 2 m high at the threshold.
constexpr double HeightAtThreshold = 2.0;

} // namespace

ATankActor::ATankActor()
{
    PrimaryActorTick.bCanEverTick = true;

    SetRootComponent(CreateDefaultSubobject<USceneComponent>(TEXT("Root")));

    Tank = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Tank"));
    Tank->SetupAttachment(GetRootComponent());

    static ConstructorHelpers::FObjectFinder<UStaticMesh>     Cube(TEXT("/Engine/BasicShapes/Cube.Cube"));
    static ConstructorHelpers::FObjectFinder<UMaterial> Basic(TEXT("/Engine/BasicShapes/BasicShapeMaterial.BasicShapeMaterial"));
    if (Cube.Succeeded())
        Tank->SetStaticMesh(Cube.Object);
    if (Basic.Succeeded())
        Tank->SetMaterial(0, Basic.Object);

    Marker = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Marker"));
    Marker->SetupAttachment(GetRootComponent());
    Marker->SetRelativeLocation(FVector(0.0, 0.0, 100.0 * HeightAtThreshold));
    Marker->SetRelativeScale3D(FVector(1.4, 1.4, 0.03));
    Marker->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    Marker->SetCastShadow(false); // over the tank
    if (Cube.Succeeded())
        Marker->SetStaticMesh(Cube.Object);
    if (Basic.Succeeded())
        Marker->SetMaterial(0, Basic.Object);

    Overflow             = CreateDefaultSubobject<UIrritatorObserverComponent>(TEXT("Overflow"));
    Overflow->OutputName = TEXT("overflow");

    Alarms             = CreateDefaultSubobject<UIrritatorObserverComponent>(TEXT("Alarms"));
    Alarms->OutputName = TEXT("alarms");
}

void ATankActor::BeginPlay()
{
    // The observers are registered first: the load connects them.
    Super::BeginPlay();

    Material = Tank->CreateDynamicMaterialInstance(0);
    SetColor(Normal);

    if (UMaterialInstanceDynamic* MarkMaterial = Marker->CreateDynamicMaterialInstance(0))
        MarkMaterial->SetVectorParameterValue(TEXT("Color"), MarkerColor);

    Overflow->OnEvent.AddDynamic(this, &ATankActor::OnOverflow);

    if (APlayerController* PC = GetWorld()->GetFirstPlayerController()) {
        EnableInput(PC);
        InputComponent->BindKey(EKeys::Up, IE_Pressed, this, &ATankActor::MoreInflow);
        InputComponent->BindKey(EKeys::Down, IE_Pressed, this, &ATankActor::LessInflow);
        InputComponent->BindKey(EKeys::R, IE_Pressed, this, &ATankActor::RestartSimulation);
        InputComponent->BindKey(EKeys::P, IE_Pressed, this, &ATankActor::TogglePause);
        InputComponent->BindKey(EKeys::Escape, IE_Pressed, this, &ATankActor::QuitDemo);
    }

    UIrritatorSubsystem* Sub = GetWorld()->GetSubsystem<UIrritatorSubsystem>();
    if (Sub and Sub->LoadProject(ProjectFile))
        Sub->Play();
}

void ATankActor::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);

    // The base of the cube on the ground.
    const double ScaleZ = FMath::Max(GetTankLevel(), 0.05) / Threshold * HeightAtThreshold;
    Tank->SetRelativeScale3D(FVector(1.0, 1.0, ScaleZ));
    Tank->SetRelativeLocation(FVector(0.0, 0.0, 50.0 * ScaleZ));
}

TArray<FString> ATankActor::GetStatusLines() const
{
    TArray<FString> Lines;

    const UIrritatorSubsystem* Sub = GetWorld()->GetSubsystem<UIrritatorSubsystem>();
    if (not Sub)
        return Lines;

    Lines.Add(FString::Printf(TEXT("t = %.1f   level = %.2f / %.0f   inflow = %.1f   alarms = %d%s"),
                              Sub->GetSimulationTime(), GetTankLevel(), Threshold, Inflow, GetAlarmCount(),
                              Sub->IsPlaying() ? TEXT("") : TEXT("   (paused)")));
    Lines.Add(TEXT("[Up/Down] inflow (below 0 the tank drains)   [R] restart   [P] pause   [Esc] quit"));

    if (const FString Error = Sub->GetLastError(); not Error.IsEmpty())
        Lines.Add(Error);

    return Lines;
}

void ATankActor::SetInflow(double Value)
{
    if (UIrritatorSubsystem* Sub = GetWorld()->GetSubsystem<UIrritatorSubsystem>())
        if (Sub->SendValue(TEXT("inflow"), Value))
            Inflow = Value;
}

double ATankActor::GetTankLevel() const
{
    const UIrritatorSubsystem* Sub = GetWorld()->GetSubsystem<UIrritatorSubsystem>();
    return Sub ? Sub->GetObserverValue(TEXT("level/value")) : 0.0;
}

int32 ATankActor::GetAlarmCount() const
{
    // The counter has no output port: its events are its new values.
    return static_cast<int32>(Alarms->GetLastValue());
}

void ATankActor::OnOverflow(double /*Time*/, int32 Port, double /*Value*/)
{
    // The port 0 of a cross is "up", the port 1 "down".
    bOverflowing = Port == 0;
    SetColor(bOverflowing ? Alarm : Normal);
}

void ATankActor::RestartSimulation()
{
    if (UIrritatorSubsystem* Sub = GetWorld()->GetSubsystem<UIrritatorSubsystem>()) {
        if (Sub->Restart()) {
            Inflow       = 1.0; // the value of the project
            bOverflowing = false;
            SetColor(Normal);
            Sub->Play();
        }
    }
}

void ATankActor::TogglePause()
{
    if (UIrritatorSubsystem* Sub = GetWorld()->GetSubsystem<UIrritatorSubsystem>())
        Sub->IsPlaying() ? Sub->Pause() : Sub->Play();
}

void ATankActor::QuitDemo()
{
    UKismetSystemLibrary::QuitGame(this, GetWorld()->GetFirstPlayerController(), EQuitPreference::Quit, false);
}

void ATankActor::SetColor(const FLinearColor& Color)
{
    if (Material)
        Material->SetVectorParameterValue(TEXT("Color"), Color);
}
