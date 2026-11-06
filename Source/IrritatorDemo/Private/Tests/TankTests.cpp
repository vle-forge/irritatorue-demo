// Copyright (c) 2026 INRAE Distributed under the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include "TankActor.h"

#include "EngineUtils.h"
#include "Misc/AutomationTest.h"
#include "Tests/AutomationCommon.h"

#if WITH_DEV_AUTOMATION_TESTS

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FIrritatorDemoTankTest, "IrritatorDemo.Tank",
                                 EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FIrritatorDemoTankTest::RunTest(const FString& Parameters)
{
    FTestWorldWrapper Wrapper;
    if (not Wrapper.CreateTestWorld(EWorldType::Game) or not Wrapper.BeginPlayInTestWorld()) {
        AddError(TEXT("no test world"));
        return false;
    }

    UWorld* World = Wrapper.GetTestWorld();

    // The game mode of the project spawns the tank, if it is the game mode of
    // the test world.
    ATankActor* Tank = nullptr;
    for (TActorIterator<ATankActor> It(World); It; ++It)
        Tank = *It;
    if (not Tank)
        Tank = World->SpawnActor<ATankActor>();

    if (not TestNotNull(TEXT("tank"), Tank))
        return false;

    const auto Run = [&](int Frames) {
        for (int i = 0; i < Frames; ++i)
            Wrapper.TickTestWorld(0.1f);
    };

    // The inflow 1 fills the tank over the threshold 5 at t = 5.
    Run(40);
    TestEqual(TEXT("level at 4"), Tank->GetTankLevel(), 4.0, 0.1);
    TestFalse(TEXT("below the threshold"), Tank->IsOverflowing());

    Run(20);
    TestEqual(TEXT("level at 6"), Tank->GetTankLevel(), 6.0, 0.1);
    TestTrue(TEXT("overflow"), Tank->IsOverflowing());
    TestEqual(TEXT("one alarm"), Tank->GetAlarmCount(), 1);

    // The drain goes below 5 at t = 6.5.
    Tank->SetInflow(-2.0);
    Run(10);
    TestEqual(TEXT("level at 7"), Tank->GetTankLevel(), 4.0, 0.1);
    TestFalse(TEXT("drained"), Tank->IsOverflowing());
    TestEqual(TEXT("still one alarm"), Tank->GetAlarmCount(), 1);

    return true;
}

#endif
