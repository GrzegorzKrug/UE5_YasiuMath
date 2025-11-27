# How to use plugin

## Use in blueprints
1. Right-click in the Blueprint graph.  
2. Search under the category **Math / Yasiu**.  
3. Select the function you need.

*All functions from library are located under same category.*

[Functions accessible in BP](https://grzegorzkrug.github.io/UE5_YasiuMath/classUYasiuMathFunctionLibrary.html)


### Squirrel RNG in Blueprints
Squirrel Object is not treated as ActorComponent, it has to be spawned from class using node
 **CreateFromClass**  
1. Right-click in the Blueprint graph.
2. Search for "Spawn From Class"
3. Put **Squirrel 13 RNG** into class parameter
4. Promote object to variable to keep it alive inside actor/component


---

## C++
To access math functions add include in your file:
```cpp
#include "YasiuMathStaticLib.h"
```
Functions are nested in name spaces: [List of functions in C++](https://grzegorzkrug.github.io/UE5_YasiuMath/namespacemembers_func.html)

---
To access the Squirrel random generator in C++, add include in your file:
```cpp
#include "SquirrelRNG.h"
```
It is **UObject**, so it must comply with unreal eninge rules for objects lifecycle.
Store ref value as any UObject if you want it to persist in game for longer.
```cpp
UPROPERTY()
TObjectPtr<USquirrel13_RNG> rng;  /* uninitialized */
```
