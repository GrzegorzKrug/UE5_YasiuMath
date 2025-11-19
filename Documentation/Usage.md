# How to use plugin

# Use in blueprints
1. Right-click in the Blueprint graph.  
2. Search under the category **Math / Yasiu**.  
3. Select the function you need.

*All functions from library are located under same category.*

### Squirrel RNG in Blueprints
Squirrel Object is not treated as ActorComponent, it has to be spawned from class using node
 **CreateFromClass**  
1. Right-click in the Blueprint graph.
2. Search for "Spawn From Class"
3. Put **Squirrel 13 RNG** into class parameter
4. Promote object to variable to keep it alive inside actor/component

---

# C++
To access Yasiu Math utilities or the Squirrel random generator in C++, include the required headers in your files:

```cpp
#include "YasiuMathStaticLib.h"
#include "SquirrelRNG.h"
```

### Full documentation
[All functions list](https://grzegorzkrug.github.io/UE5_YasiuMath/functions_func.html)
