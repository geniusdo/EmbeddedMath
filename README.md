# EmbeddedMath
<img src="https://github.com/geniusdo/EmbeddedMath/blob/main/doc/ai_generated_logo" width="30%" height="auto" alt="LOGO">   
  
A C++ header only linear algebra math library for embedded device.

## Purpose:

Bringing a smooth **linear algebra** development experience, with an **Eigen-like interface**, even on **embedded devices**!  

## Usage:
By simply aliasing `Eigen` to `EmbeddedMath`, you can use the same API as Eigen on microcontrollers with C++ support enabled.

```cpp
#include <EmbeddedMath.hpp>

namespace Eigen = EmbeddedMath;  // Alias EmbeddedMath to Eigen

int main() {
    // Create a quaternion (w, x, y, z)
    Eigen::Quaternionf q(0.707f, 0.0f, 0.707f, 0.0f);  // 90-degree rotation around the Y axis

    // Convert quaternion to rotation matrix
    Eigen::Matrix3f rotation_matrix = q.toRotationMatrix();

    // Rotate a vector
    Eigen::Vector3f v(1.0f, 0.0f, 0.0f);
    Eigen::Vector3f rotated_v = rotation_matrix * v;

    return 0;
}
```

Simply drag and drop the `EmbeddedMath.hpp` file into your project, and you're all set to get started!

## Dependencies:
- C++17 or later (test with`arm-none-eabi-g++ 10.3.1`)
- Posix math library (e.g., `math.h`)
