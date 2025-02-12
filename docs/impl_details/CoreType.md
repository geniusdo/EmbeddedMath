## Implementation Details of EmbeddedCoreType

`EmbeddedCoreType` is the fundamental matrix data structure. 
   
It stores a 2D matrix data in **Column-major** order. Other attributes including the number of rows and columns, and the number of elements in the class are statically pre-allocated.   

Since the library is optimized for speed, we don't make additional checks for the validity of the matrix data. Users should be responsible to the inputs.  

Different from `Eigen` library, all data in `EmbeddedMath` are statically allocated, and **No lazy evaluation** will be performed. 

Note that `EmbeddedMath` library is designed for Embedded devices. Therefore we don't have good support for large matrix.  

### 1. `EmbeddedCoreType` Class

#### Initialization
For most embedded devices, `<iostream>` is not supported. Therefore, we don't provide stream operators like `<<` or `>>`. Users can initalize a matrix by either predefined matrix or by manually setting the elements.
```cpp
Matrix3d mat1 = Matrix3d::Identity();
Matrix3d mat2 = Matrix3d(0);
Matrix3d mat3, mat4;
mat3(0, 0) = 1;
mat4.block<2,2>(0,0) = Matrix2d::Identity();
Vecotr3d vec1 = Vector3d(1, 2, 3);
... ...
```
#### constructor
Currently, only copy constructor is supported.  
```cpp
EmbeddedCoreType()
{
    memset(Elements, (ScalarType)0, sizeof(ScalarType) * size);
}

EmbeddedCoreType(const EmbeddedCoreType &other)
{
    memcpy(this->Elements, other.data(), sizeof(ScalarType) * size);
}
```


### 2. Frequently Used Types
The predefined classes for frequently used matrix types are listed below.
```cpp
template <typename T, int rows, int cols>
using Matrix = EmbeddedCoreType<T, rows, cols>;

template <typename T>
using Quaternion = EmbeddedQuaternion<T>;

using Vector2f = EmbeddedCoreType<float, 2, 1>;
using Vector3f = EmbeddedCoreType<float, 3, 1>;
using Vector4f = EmbeddedCoreType<float, 4, 1>;

using Quaternionf = EmbeddedQuaternion<float>;

using Matrix2f = EmbeddedCoreType<float, 2, 2>;
using Matrix3f = EmbeddedCoreType<float, 3, 3>;
using Matrix4f = EmbeddedCoreType<float, 4, 4>;

using Vector2d = EmbeddedCoreType<double, 2, 1>;
using Vector3d = EmbeddedCoreType<double, 3, 1>;
using Vector4d = EmbeddedCoreType<double, 4, 1>;

using Quaterniond = EmbeddedQuaternion<double>;

using Matrix2d = EmbeddedCoreType<double, 2, 2>;
using Matrix3d = EmbeddedCoreType<double, 3, 3>;
using Matrix4d = EmbeddedCoreType<double, 4, 4>;

constexpr auto AngleAxisf = AngleAxis<float>;
constexpr auto AngleAxisd = AngleAxis<double>;
```