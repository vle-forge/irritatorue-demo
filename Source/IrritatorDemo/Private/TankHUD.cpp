// Copyright (c) 2026 INRAE Distributed under the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include "TankHUD.h"

#include "TankActor.h"

#include "Engine/Engine.h"
#include "EngineUtils.h"

void ATankHUD::DrawHUD()
{
    Super::DrawHUD();

    TActorIterator<ATankActor> It(GetWorld());
    if (not It)
        return;

    UFont* Font = GEngine->GetMediumFont();
    float  Y    = 24.f;

    for (const FString& Line : It->GetStatusLines()) {
        // A shadow: readable on the sky and on the floor.
        DrawText(Line, FLinearColor::Black, 25.f, Y + 1.f, Font, 1.4f);
        DrawText(Line, FLinearColor::White, 24.f, Y, Font, 1.4f);
        Y += 28.f;
    }
}
