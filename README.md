# IrritatorDemo

A playable demo of [IrritatorUE](https://github.com/vle-forge/irritator), the
Unreal Engine 5 plug-in of the irritator DEVS simulator.

A tank is filled by a simulation (`Content/Irritator/irtue-demo/tank.pirt`):

- `inflow`, a constant marked *configurable*, is an input of the game: the
  keys change its value with `SendValue()`;
- `level`, a QSS1 integrator, is observed: the height of the tank follows its
  value, read each frame with `GetObserverValue()`;
- `overflow`, a cross of the level 5, and `alarms`, a counter, are marked
  *observable*: they are outputs of the game, received by two
  `UIrritatorObserverComponent`. The tank turns red when the level goes over 5.

Keys: `Up` / `Down` change the inflow (below 0 the tank drains), `R`
restarts, `P` pauses.

The scene is built in C++ by `ATankGameMode` in the empty map of the engine:
the repository has no map and no asset.

## Build and run

The plug-in is the repository `irritatorue`, cloned beside this one:
`Plugins/IrritatorUE` is a link to `../../irritatorue`.

```sh
UE=~/apps/UnrealEngine-5.7
$UE/Engine/Build/BatchFiles/Linux/Build.sh IrritatorDemoEditor Linux Development -Project=$PWD/IrritatorDemo.uproject
$UE/Engine/Binaries/Linux/UnrealEditor $PWD/IrritatorDemo.uproject -game
```

The test of the demo:

```sh
$UE/Engine/Binaries/Linux/UnrealEditor-Cmd $PWD/IrritatorDemo.uproject \
  -ExecCmds="Automation RunTests IrritatorDemo; Quit" -unattended -nullrhi
```
