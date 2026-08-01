\page baseuse Usage guide

# For Blueprints

## Functions
1. Right-click in the Blueprint graph.  
2. Type category label **Math|Yasiu**.
3. Select the function you need.

#### [**Function list**](classUYasiuMathFunctionLibrary.html)

<!-- #### [**Functions accessible in BP**](class_u_yasiu_math_function_library.html) -->

---

## Random number generators

Currently most classes are made as **UObjects** for coding flexibility. 

### UObjects
They are spawned a bit different than actor components.

Squirrel Object is not treated as ActorComponent, it has to be spawned from class using node
 **CreateFromClass**  
1. Right-click in the Blueprint graph.
2. Search for "Spawn From Class"
3. Put **Squirrel 13 RNG** into class parameter (or PCG)
4. Promote object to variable to keep it alive
	- This is only required to keep object alive during gameplay and prevent UE from destroying it.


### Actor components
1. Create actor or use existing one
2. Open actors editor window
3. In tree structure click "Add" button
4. Find component u want to add

\image html Images/AddingComponent.png "Adding component" width=300px

## [**List of unreal objects**](unreal.html)

---
---

# For coding in C++
To use math functions include this in your file:
```cpp
"Functions in vanilla C++";
#include "YasiuMathLib.h"

"Functions that use Unreal types";
#include "YasiuMathUnrealLib.h"
```

<!-- Local redirector -->
#### List of all [**functions for C++**](namespacemembers_func.html) grouped into: [**Name spaces**](namespaces.html)

---
To access one of the RNG in C++, add include in your file:
```cpp
#include "SquirrelRNG.h"
/* or */
#include "PCG_RNG.h"
```
It is **UObject**, so it must comply with unreal engine rules for objects lifecycle.
Store ref value as any UObject if you want it to persist in game for longer.

```cpp
/* Declaration in class, initialize this in CDO or during gameplay */
UPROPERTY()
TObjectPtr<USquirrel13_RNG> rng;
UPROPERTY()
TObjectPtr<UPCG32_RNG> rng;
```

Squirrel13 has also ActorComponent work with network replication.

### [**All Classes list**](annotated.html)
