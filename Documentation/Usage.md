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
4. Promote object to variable to keep it alive
	- This is only required to keep object alive during gameplay and prevent UE from destroying it.

---

## C++
To use math functions include this in your file:
```cpp
// Functions in c++
#include "YasiuMathLib.h"

// Functions that use UE types
#include "YasiuMathUnrealLib.h"
```

[List of all functions for C++](https://grzegorzkrug.github.io/UE5_YasiuMath/namespacemembers_func.html)

Functions are grouped into namespaces: [Name spaces](https://grzegorzkrug.github.io/UE5_YasiuMath/namespaces.html)

---
To access the Squirrel random generator in C++, add include in your file:
```cpp
#include "SquirrelRNG.h"
```
It is **UObject**, so it must comply with unreal engine rules for objects lifecycle.
Store ref value as any UObject if you want it to persist in game for longer.
```cpp
/* Declaration in class, initialize in CDO */
UPROPERTY()
TObjectPtr<USquirrel13_RNG> rng;
```
