// Copyright (c) 2026 INRAE Distributed under the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"

#include "TankGameMode.generated.h" // always last

class ACameraActor;

/**
 * Builds the scene of the demo in any map (the empty map of the engine by
 * default): a tank, a sky, a sun, a floor and a fixed camera. Without map and
 * asset, the demo is only C++ and the files of Content/Irritator.
 */
UCLASS()
class IRRITATORDEMO_API ATankGameMode : public AGameModeBase
{
    GENERATED_BODY()

public:
    ATankGameMode();

    virtual void StartPlay() override;
    virtual void PostLogin(APlayerController* NewPlayer) override;

private:
    /// The fixed camera becomes the view of the player.
    void UseCamera(APlayerController* PC);

    UPROPERTY(Transient)
    TObjectPtr<ACameraActor> Camera;
};
