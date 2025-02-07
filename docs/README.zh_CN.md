# EmbeddedMath
<p align="left">
  <img src="https://github.com/geniusdo/EmbeddedMath/blob/main/docs/ai_generated_logo.png" width="20%" height="auto" alt="LOGO">
</p>   

一个专为嵌入式设备设计的轻量级线性代数库.  

## 目标:

为嵌入式设备提供一套**轻量级**的**线性代数**库，并尽可能提供与**Eigen**类似的API。  

## 特点:
- **<span style="color:orange">Eigen-like API</span>**: Eigen风格API,但仅使用静态内存.
- **<span style="color:lightblue">轻量级</span>**: 仅有一个头文件,且只依赖C标准库.
- **<span style="color:red">极致优化</span>**: 精心为速度和资源限制应用设计,秉承C++ **Zero-Cost Abstraction** 设计哲学.
- **<span style="color:cyan">跨平台</span>**: 只要支持C++17即可编译.
- **<span style="color:pink">开箱即用</span>**: 轻轻把头文件拖进你的工程,即可愉快使用.
- **<span style="color:green">测试覆盖</span>**: 全面的单元测试.



## 使用方法:
你可以将`EmbeddedMath`重命名为`Eigen`，然后在单片机上像使用Eigen一样使用它.  

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
简单地,将`EmbeddedMath.hpp`文件拖入你的工程,即可开始使用.  

## 案例: 
一个经典的使用本库的例子是实现卡尔曼滤波器,可以在[Embedded-ESKF](https://github.com/geniusdo/Embedded-ESKF)中找到.  

## 依赖:
- C++17 or later (test with`arm-none-eabi-g++ 10.3.1`)
- Posix math library (e.g., `math.h`)


## License:
The code is under Apache License 2.0.

#### 有任何问题或者建议,欢迎提交issue或者PR.
*如果本库有帮到您,给我点个star吧!* 