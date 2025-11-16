# Using the Yasiu Math (Blueprints & C++)

## Blueprints
To find all available functions:

1. Right-click in the Blueprint graph.  
2. Search under the category **"Math / Yasiu"**.  
3. Select the function you need.

### Squirrel in Blueprints
To spawn the Squirrel object in Blueprints, use:  
**CreateFromClass**  


---

## C++ Integration
To access Yasiu math utilities or the Squirrel random generator in C++, include the required headers in your files:

```cpp
#include "YasiuMathStaticLib.h"
#include "SquirrelRNG.h"
```