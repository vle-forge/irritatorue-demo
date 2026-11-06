// Copyright (c) 2026 INRAE Distributed under the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "TankActor.generated.h" // always last

class UIrritatorObserverComponent;
class UMaterialInstanceDynamic;
class UStaticMeshComponent;
class UTextRenderComponent;

/**
 * A tank filled by the simulation of Content/Irritator/irtue-demo/tank.pirt:
 * the inflow (an input) fills the level (an integrator, read each frame); the
 * overflow (a cross of the level 5, an output) turns the tank red, and the
 * alarms (a counter, an output) count the overflows.
 *
 * Keys: Up and Down change the inflow, R restarts, P pauses.
 */
UCLASS()
class IRRITATORDEMO_API ATankActor : public AActor
{
    GENERATED_BODY()

public:
    ATankActor();

    /// Relative to Content/Irritator.
    UPROPERTY(EditAnywhere, Category = "Irritator Demo")
    FString ProjectFile = TEXT("irtue-demo/tank.pirt");

    UPROPERTY(EditAnywhere, Category = "Irritator Demo")
    double InflowStep = 0.5;

    UFUNCTION(BlueprintCallable, Category = "Irritator Demo")
    void SetInflow(double Value);

    UFUNCTION(BlueprintPure, Category = "Irritator Demo")
    double GetInflow() const { return Inflow; }

    UFUNCTION(BlueprintPure, Category = "Irritator Demo")
    double GetTankLevel() const;

    UFUNCTION(BlueprintPure, Category = "Irritator Demo")
    bool IsOverflowing() const { return bOverflowing; }

    UFUNCTION(BlueprintPure, Category = "Irritator Demo")
    int32 GetAlarmCount() const;

    virtual void BeginPlay() override;
    virtual void Tick(float DeltaSeconds) override;

private:
    UFUNCTION()
    void OnOverflow(double Time, int32 Port, double Value);

    void MoreInflow() { SetInflow(Inflow + InflowStep); }
    void LessInflow() { SetInflow(Inflow - InflowStep); }
    void RestartSimulation();
    void TogglePause();
    void SetColor(const FLinearColor& Color);

    UPROPERTY(VisibleAnywhere, Category = "Irritator Demo")
    TObjectPtr<UStaticMeshComponent> Tank;

    UPROPERTY(VisibleAnywhere, Category = "Irritator Demo")
    TObjectPtr<UTextRenderComponent> Label;

    UPROPERTY(VisibleAnywhere, Category = "Irritator Demo")
    TObjectPtr<UIrritatorObserverComponent> Overflow;

    UPROPERTY(VisibleAnywhere, Category = "Irritator Demo")
    TObjectPtr<UIrritatorObserverComponent> Alarms;

    UPROPERTY(Transient)
    TObjectPtr<UMaterialInstanceDynamic> Material;

    double Inflow       = 1.0;
    bool   bOverflowing = false;
};
