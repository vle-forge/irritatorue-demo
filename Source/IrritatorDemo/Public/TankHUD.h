// Copyright (c) 2026 INRAE Distributed under the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"

#include "TankHUD.generated.h" // always last

/// Shows the state of the tank and the keys on the screen.
UCLASS()
class IRRITATORDEMO_API ATankHUD : public AHUD
{
    GENERATED_BODY()

public:
    virtual void DrawHUD() override;
};
