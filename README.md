# EmbeddedMath
<p align="left">
  <img src="docs/ai_generated_logo.png" width="20%" height="auto" alt="LOGO">
</p>   

[English](README.md) | [简体中文](docs/README.zh_CN.md)  
[Implementation Details](docs/impl_details//CoreType.md)

A C++ header only linear algebra math library for embedded device.



## Purpose:

Bringing a smooth **linear algebra** development experience, with an **Eigen-like interface**, even on **embedded devices**!

## Features:
- **<span style="color:orange">Eigen-like API</span>**: Use the same API as Eigen, but with only static memory allocation.
- **<span style="color:lightblue">Small footprint</span>**: Only a few kilobytes of code, with no dependencies on external libraries.
- **<span style="color:red">Fast performance</span>**: Optimized for speed and efficiency, with template metaprogramming.
- **<span style="color:cyan">Cross-platform compatibility</span>**: Works on any platform that supports C++17 or later.
- **<span style="color:pink">Easy to use</span>**: Simply include the header file in your project and start using the API.
- **<span style="color:green">Test coverage</span>**: The library is thoroughly tested and has a high test coverage rate.



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

    // Create a transformation matrix
    Eigen::Matrix4f transformation_matrix = Eigen::Matrix4f::Identity();
    transformation_matrix.block<3, 3>(0, 0) = rotation_matrix;

    return 0;
}
```

Simply drag and drop the `EmbeddedMath.hpp` file into your project, and you're all set to get started!

## Example: 
A typical example of this library is to implement a kalman filter, which can be found at [Embedded-ESKF](https://github.com/geniusdo/Embedded-ESKF).

## Dependencies:
- C++17 or later (test with`arm-none-eabi-g++ 10.3.1`)
- Posix math library (e.g., `math.h`)


## License:
The code is under Apache License 2.0.

#### Any issues or suggestions, please open an issue or PR.
*Give me a star if you like it!* 