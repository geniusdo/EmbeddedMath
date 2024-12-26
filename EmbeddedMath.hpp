// Copyright 2024 Liu Chuangye @ chuangyeliu0206@gmail.com
// Apache Licence 2.0
#ifndef EMBEDDEDMATH_HPP_
#define EMBEDDEDMATH_HPP_

#include "math.h"
#include "string.h"
#define FLOAT_EPSILON 1.1920929e-7f

namespace EmbeddedTypes
{
    template <typename ScalarType, int rows, int cols>
    class EmbeddedCoreType
    {
    protected:
        ScalarType Elements[rows * cols];
        static constexpr int size = rows * cols;

    public:
        using Scalar = ScalarType;
        static constexpr int RowsAtCompileTime = rows;
        static constexpr int ColsAtCompileTime = cols;
        static constexpr int MaxRankAtCompileTime = (RowsAtCompileTime < ColsAtCompileTime) ? RowsAtCompileTime : ColsAtCompileTime;
        static constexpr int MaxDimAtCompileTime = (RowsAtCompileTime > ColsAtCompileTime) ? RowsAtCompileTime : ColsAtCompileTime;

        EmbeddedCoreType()
        {
            memset(Elements, (ScalarType)0, sizeof(ScalarType) * size);
        }

        EmbeddedCoreType(const EmbeddedCoreType &other)
        {
            memcpy(this->Elements, other.data(), sizeof(ScalarType) * size);
        }

        EmbeddedCoreType(const ScalarType value)
        {
            for (int i = 0; i < size; ++i)
            {
                Elements[i] = value;
            }
        }

        EmbeddedCoreType(const ScalarType a1, const ScalarType a2)
        {
            static_assert(MaxDimAtCompileTime >= 2);
            Elements[0] = a1;
            Elements[1] = a2;
        }

        EmbeddedCoreType(const ScalarType a1, const ScalarType a2, const ScalarType a3)
        {
            static_assert(MaxDimAtCompileTime >= 3);
            Elements[0] = a1;
            Elements[1] = a2;
            Elements[2] = a3;
        }

        EmbeddedCoreType(const ScalarType a1, const ScalarType a2, const ScalarType a3, const ScalarType a4)
        {
            static_assert(MaxDimAtCompileTime >= 4);
            Elements[0] = a1;
            Elements[1] = a2;
            Elements[2] = a3;
            Elements[3] = a4;
        }

        ~EmbeddedCoreType() {}

        inline ScalarType &x()
        {
            static_assert(MaxRankAtCompileTime == 1 && MaxDimAtCompileTime >= 1);
            return this->Elements[0];
        }
        inline const ScalarType &x() const
        {
            static_assert(MaxRankAtCompileTime == 1 && MaxDimAtCompileTime >= 1);
            return this->Elements[0];
        }

        inline ScalarType &y()
        {
            static_assert(MaxRankAtCompileTime == 1 && MaxDimAtCompileTime >= 2);
            return this->Elements[1];
        }
        inline const ScalarType &y() const
        {
            static_assert(MaxRankAtCompileTime == 1 && MaxDimAtCompileTime >= 2);
            return this->Elements[1];
        }

        inline ScalarType &z()
        {
            static_assert(MaxRankAtCompileTime == 1 && MaxDimAtCompileTime >= 3);
            return this->Elements[2];
        }
        inline const ScalarType &z() const
        {
            static_assert(MaxRankAtCompileTime == 1 && MaxDimAtCompileTime >= 3);
            return this->Elements[2];
        }

        inline ScalarType &operator()(int index)
        {
            return Elements[index];
        }

        inline const ScalarType &operator()(int index) const
        {
            return Elements[index];
        }

        inline ScalarType &operator()(int row, int col)
        {
            return Elements[col * rows + row];
        }

        inline const ScalarType &operator()(int row, int col) const
        {
            return Elements[col * rows + row];
        }

        inline ScalarType *data()
        {
            return Elements;
        }

        inline const ScalarType *data() const
        {
            return Elements;
        }

        template <int length>
        inline const EmbeddedCoreType<ScalarType, length, 1> head() const
        {
            EmbeddedCoreType<ScalarType, length, 1> result;
            for (int i = 0; i < length; i++)
            {
                result(i) = Elements[i];
            }
            return result;
        }

        template <int length>
        inline const EmbeddedCoreType<ScalarType, length, 1> tail() const
        {
            EmbeddedCoreType<ScalarType, length, 1> result;
            for (int i = 0; i < length; i++)
            {
                result(i) = Elements[size - length + i];
            }
            return result;
        }

        inline bool operator==(const EmbeddedCoreType &other) const
        {
            for (int i = 0; i < size; i++)
            {
                if (Elements[i] != other(i))
                    return false;
            }
            return true;
        }

        inline bool operator!=(const EmbeddedCoreType &other) const
        {
            for (int i = 0; i < size; i++)
            {
                if (Elements[i] != other(i))
                    return true;
            }
            return false;
        }

        inline EmbeddedCoreType operator+(const EmbeddedCoreType &other) const
        {
            EmbeddedCoreType result;
            for (int i = 0; i < size; i++)
            {
                result(i) = Elements[i] + other(i);
            }
            return result;
        }

        inline EmbeddedCoreType operator-(const EmbeddedCoreType &other) const
        {
            EmbeddedCoreType result;
            for (int i = 0; i < size; i++)
            {
                result(i) = Elements[i] - other(i);
            }
            return result;
        }

        inline EmbeddedCoreType operator*(const ScalarType factor) const
        {
            EmbeddedCoreType result;
            for (int i = 0; i < size; i++)
            {
                result(i) = Elements[i] * factor;
            }
            return result;
        }

        friend EmbeddedCoreType operator*(ScalarType factor, const EmbeddedCoreType &matrix)
        {
            return matrix * factor;
        }

        inline EmbeddedCoreType operator/(const ScalarType factor) const
        {
            EmbeddedCoreType result;
            for (int i = 0; i < size; i++)
            {
                result(i) = Elements[i] / factor;
            }
            return result;
        }

        inline EmbeddedCoreType &operator+=(const EmbeddedCoreType &other)
        {
            for (int i = 0; i < size; i++)
            {
                Elements[i] += other(i);
            }
            return *this;
        }

        inline EmbeddedCoreType &operator-=(const EmbeddedCoreType &other)
        {
            for (int i = 0; i < size; i++)
            {
                Elements[i] -= other(i);
            }
            return *this;
        }

        inline EmbeddedCoreType &operator*=(const ScalarType factor)
        {
            for (int i = 0; i < size; i++)
            {
                Elements[i] *= factor;
            }
            return *this;
        }

        inline EmbeddedCoreType &operator/=(const ScalarType factor)
        {
            for (int i = 0; i < size; i++)
            {
                Elements[i] /= factor;
            }
            return *this;
        }

        inline EmbeddedCoreType<ScalarType, cols, rows> transpose() const
        {
            EmbeddedCoreType<ScalarType, cols, rows> result;
            for (int i = 0; i < rows; i++)
            {
                for (int j = 0; j < cols; j++)
                {
                    result(j, i) = Elements[j * cols + i];
                }
            }
            return result;
        }

        inline void setZero()
        {
            memset(Elements, 0, size * sizeof(ScalarType));
            return;
        }

        static inline EmbeddedCoreType Zero()
        {
            EmbeddedCoreType result;
            memset(result.data(), (ScalarType)0, size * sizeof(ScalarType));
            return result;
        }

        static inline EmbeddedCoreType Ones()
        {
            EmbeddedCoreType result;
            for (int i = 0; i < size; i++)
            {
                result(i) = static_cast<ScalarType>(1);
            }
            return result;
        }

        inline ScalarType norm() const
        {
            ScalarType result = 0;
            for (int i = 0; i < size; i++)
            {
                result += Elements[i] * Elements[i];
            }
            return sqrt(result);
        }

        inline EmbeddedCoreType normalized() const
        {
            EmbeddedCoreType result;
            ScalarType _norm = norm();
            for (int i = 0; i < size; i++)
            {
                result(i) = Elements[i] / _norm;
            }
            return result;
        }

        inline void normalize()
        {
            ScalarType _norm = norm();
            for (int i = 0; i < size; i++)
            {
                Elements[i] /= _norm;
            }
            return;
        }

        inline ScalarType trace() const
        {
            ScalarType result = 0;
            for (int i = 0; i < MaxRankAtCompileTime; i++)
            {
                result += this->Elements[i * cols + i];
            }
            return result;
        }

        inline ScalarType dot(const EmbeddedCoreType &other) const
        {
            ScalarType result = 0;
            for (int i = 0; i < size; i++)
            {
                result += this->Elements[i] * other(i);
            }
            return result;
        }

        inline EmbeddedCoreType cross(const EmbeddedCoreType &other) const
        {
            EmbeddedCoreType result;
            if constexpr (RowsAtCompileTime == 2 && ColsAtCompileTime == 1)
            {
                result(0) = this->Elements[1] * other(0);
                result(1) = -this->Elements[0] * other(0);
            }
            else if constexpr (RowsAtCompileTime == 3 && ColsAtCompileTime == 1)
            {
                result(0) = this->Elements[1] * other(2) - this->Elements[2] * other(1);
                result(1) = this->Elements[2] * other(0) - this->Elements[0] * other(2);
                result(2) = this->Elements[0] * other(1) - this->Elements[1] * other(0);
            }
            return result;
        }

        inline void setIdentity()
        {
            setZero();
            for (int i = 0; i < MaxRankAtCompileTime; i++)
            {
                this->Elements[i * cols + i] = static_cast<ScalarType>(1);
            }
            return;
        }

        static inline EmbeddedCoreType Identity()
        {
            EmbeddedCoreType result;
            for (int i = 0; i < MaxRankAtCompileTime; i++)
            {
                result.data()[i * cols + i] = static_cast<ScalarType>(1);
            }
            return result;
        }

        inline EmbeddedCoreType inverse() const
        {
            EmbeddedCoreType result;
            if constexpr (RowsAtCompileTime == 2 && ColsAtCompileTime == 2)
            {
                // a b
                // c d
                // det = a * d - b * c
                ScalarType det = this->Elements[0] * this->Elements[3] - this->Elements[1] * this->Elements[2];
                if (abs(det) < FLOAT_EPSILON)
                {
                    return EmbeddedCoreType::Zero();
                }
                ScalarType invDet = (ScalarType)1 / det;

                result(0, 0) = this->Elements[3] * invDet;
                result(0, 1) = -this->Elements[2] * invDet;
                result(1, 0) = -this->Elements[1] * invDet;
                result(1, 1) = this->Elements[0] * invDet;
            }
            else if constexpr (RowsAtCompileTime == 3 && ColsAtCompileTime == 3)
            {
                // a b c
                // d e f
                // g h i
                // det = a * e * i - a * f * h - b * d * i + b * f * g + c * d * h - c * e * g
                ScalarType det = this->Elements[0] * this->Elements[4] * this->Elements[8] -
                                 this->Elements[0] * this->Elements[7] * this->Elements[5] -
                                 this->Elements[3] * this->Elements[1] * this->Elements[8] +
                                 this->Elements[3] * this->Elements[7] * this->Elements[2] +
                                 this->Elements[6] * this->Elements[1] * this->Elements[5] -
                                 this->Elements[6] * this->Elements[4] * this->Elements[2];

                if (det == 0)
                {
                    return EmbeddedCoreType::Zero();
                }
                ScalarType invDet = (ScalarType)1.0 / det;

                result(0, 0) = (this->Elements[4] * this->Elements[8] - this->Elements[5] * this->Elements[7]) * invDet;
                result(0, 1) = (this->Elements[6] * this->Elements[5] - this->Elements[3] * this->Elements[8]) * invDet;
                result(0, 2) = (this->Elements[3] * this->Elements[7] - this->Elements[6] * this->Elements[4]) * invDet;

                result(1, 0) = (this->Elements[2] * this->Elements[7] - this->Elements[1] * this->Elements[8]) * invDet;
                result(1, 1) = (this->Elements[0] * this->Elements[8] - this->Elements[2] * this->Elements[6]) * invDet;
                result(1, 2) = (this->Elements[1] * this->Elements[6] - this->Elements[0] * this->Elements[7]) * invDet;

                result(2, 0) = (this->Elements[1] * this->Elements[5] - this->Elements[2] * this->Elements[4]) * invDet;
                result(2, 1) = (this->Elements[2] * this->Elements[3] - this->Elements[0] * this->Elements[5]) * invDet;
                result(2, 2) = (this->Elements[0] * this->Elements[4] - this->Elements[1] * this->Elements[3]) * invDet;
            }
            else if constexpr (RowsAtCompileTime == 4 && ColsAtCompileTime == 4)
            {
                // TODO : implement
            }
            else
            {
                // TODO : implement
            }
            return result;
        }

        template <typename T, int R1, int C1_R2, int C2>
        friend EmbeddedCoreType<T, R1, C2> operator*(
            const EmbeddedCoreType<T, R1, C1_R2> &lhs,
            const EmbeddedCoreType<T, C1_R2, C2> &rhs);

        inline EmbeddedCoreType<ScalarType, MaxDimAtCompileTime, MaxDimAtCompileTime> asDiagonal() const
        {
            static_assert(MaxRankAtCompileTime == 1);

            EmbeddedCoreType<ScalarType, MaxDimAtCompileTime, MaxDimAtCompileTime> result;
            for (int i = 0; i < MaxDimAtCompileTime; i++)
            {
                result(i, i) = this->Elements[i];
            }
            return result;
        }

        bool isApprox(const EmbeddedCoreType &other, const ScalarType &epsilon = FLOAT_EPSILON) const
        {
            ScalarType L2_Dis = (*this - other).norm();
            return L2_Dis <= epsilon;
        }

        //! do nothing
        inline EmbeddedCoreType eval()
        {
            return *this;
        }
    };

    template <typename T, int R1, int C1_R2, int C2>
    EmbeddedCoreType<T, R1, C2> operator*(
        const EmbeddedCoreType<T, R1, C1_R2> &lhs,
        const EmbeddedCoreType<T, C1_R2, C2> &rhs)
    {
        EmbeddedCoreType<T, R1, C2> result;
        for (int i = 0; i < R1; ++i)
        {
            for (int j = 0; j < C2; ++j)
            {
                for (int k = 0; k < C1_R2; ++k)
                {
                    result(i, j) += lhs(i, k) * rhs(k, j);
                }
            }
        }
        return result;
    }
    // Partial Specialize Quaternion
    template <typename ScalarType>
    class EmbeddedQuaternion : public EmbeddedCoreType<ScalarType, 4, 1>
    {
    protected:
        using EmbeddedCoreType<ScalarType, 4, 1>::Elements;

    public:
        using BaseType = EmbeddedCoreType<ScalarType, 4, 1>;

        EmbeddedQuaternion() : BaseType() {}

        EmbeddedQuaternion(const ScalarType w, const ScalarType x, const ScalarType y, const ScalarType z)
        {
            this->Elements[0] = x;
            this->Elements[1] = y;
            this->Elements[2] = z;
            this->Elements[3] = w; // w
        }

        EmbeddedQuaternion(const BaseType &other)
        {
            this->Elements[0] = other(0);
            this->Elements[1] = other(1);
            this->Elements[2] = other(2);
            this->Elements[3] = other(3); // w
        }

        EmbeddedQuaternion(const EmbeddedQuaternion<ScalarType> &other)
            : BaseType(other) {}

        EmbeddedQuaternion(const EmbeddedCoreType<ScalarType, 3, 3> &rot)
        {
            ScalarType trace = rot.trace();
            if (trace > 0)
            {
                ScalarType s = 0.5 / sqrt(trace + 1.0);
                this->Elements[3] = 0.25 / s;
                this->Elements[0] = (rot(2, 1) - rot(1, 2)) * s;
                this->Elements[1] = (rot(0, 2) - rot(2, 0)) / s;
                this->Elements[2] = (rot(1, 0) - rot(0, 1)) / s;
            }
            else
            {
                if (rot(0, 0) > rot(1, 1) && rot(0, 0) > rot(2, 2))
                {
                    ScalarType s = sqrt(1.0f + rot(0, 0) - rot(1, 1) - rot(2, 2)) * 2;
                    this->Elements[3] = (rot(2, 1) - rot(1, 2)) / s;
                    this->Elements[0] = 0.25f * s;
                    this->Elements[1] = (rot(0, 1) + rot(1, 0)) / s;
                    this->Elements[2] = (rot(0, 2) + rot(2, 0)) / s;
                }
                else if (rot(1, 1) > rot(2, 2))
                {
                    ScalarType s = sqrt(1.0f + rot(1, 1) - rot(0, 0) - rot(2, 2)) * 2;
                    this->Elements[3] = (rot(0, 2) - rot(2, 0)) / s;
                    this->Elements[0] = (rot(0, 1) + rot(1, 0)) / s;
                    this->Elements[1] = 0.25f * s;
                    this->Elements[2] = (rot(1, 2) + rot(2, 1)) / s;
                }
                else
                {
                    ScalarType s = sqrt(1.0f + rot(2, 2) - rot(0, 0) - rot(1, 1)) * 2;
                    this->Elements[3] = (rot(1, 0) - rot(0, 1)) / s;
                    this->Elements[0] = (rot(0, 2) + rot(2, 0)) / s;
                    this->Elements[1] = (rot(1, 2) + rot(2, 1)) / s;
                    this->Elements[2] = 0.25f * s;
                }
            }
            this->normalize();
        }

        ~EmbeddedQuaternion() {};

        inline ScalarType &w()
        {
            return this->Elements[3];
        }
        inline const ScalarType &w() const
        {
            return this->Elements[3];
        }

        inline EmbeddedCoreType<ScalarType, 3, 1> &vec()
        {
            return *reinterpret_cast<EmbeddedCoreType<ScalarType, 3, 1> *>(&this->Elements[0]);
        }

        const inline EmbeddedCoreType<ScalarType, 3, 1> &vec() const
        {
            return *reinterpret_cast<const EmbeddedCoreType<ScalarType, 3, 1> *>(&this->Elements[0]);
        }

        inline EmbeddedQuaternion conjugate() const
        {
            EmbeddedQuaternion result;
            result.w() = this->w();
            result.x() = -this->x();
            result.y() = -this->y();
            result.z() = -this->z();
            return result;
        }

        inline EmbeddedQuaternion inverse() const
        {
            EmbeddedQuaternion result;
            result.w() = this->w();
            result.x() = -this->x();
            result.y() = -this->y();
            result.z() = -this->z();
            result.normalize();
            return result;
        }

        friend inline EmbeddedQuaternion operator*(const EmbeddedQuaternion &left, const EmbeddedQuaternion &right)
        {
            EmbeddedQuaternion result;
            result.w() = left.w() * right.w() - left.x() * right.x() - left.y() * right.y() - left.z() * right.z();
            result.x() = left.w() * right.x() + left.x() * right.w() + left.y() * right.z() - left.z() * right.y();
            result.y() = left.w() * right.y() - left.x() * right.z() + left.y() * right.w() + left.z() * right.x();
            result.z() = left.w() * right.z() + left.x() * right.y() - left.y() * right.x() + left.z() * right.w();
            return result;
        }

        inline EmbeddedCoreType<ScalarType, 3, 3> toRotationMatrix() const
        {
            ScalarType w = this->w();
            ScalarType x = this->x();
            ScalarType y = this->y();
            ScalarType z = this->z();
            EmbeddedCoreType<ScalarType, 3, 3> result;
            result(0, 0) = (ScalarType)1.0f - (ScalarType)2.0f * (y * y + z * z);
            result(0, 1) = (ScalarType)2.0f * (x * y - w * z);
            result(0, 2) = (ScalarType)2.0f * (x * z + w * y);

            result(1, 0) = (ScalarType)2.0f * (x * y + w * z);
            result(1, 1) = (ScalarType)1.0f - (ScalarType)2.0f * (x * x + z * z);
            result(1, 2) = (ScalarType)2.0f * (y * z - w * x);

            result(2, 0) = (ScalarType)2.0f * (x * z - w * y);
            result(2, 1) = (ScalarType)2.0f * (y * z + w * x);
            result(2, 2) = (ScalarType)1.0f - (ScalarType)2.0f * (x * x + y * y);
            return result;
        }

        inline EmbeddedCoreType<ScalarType, 3, 1> toEulerAngles() const
        {
            const ScalarType singularityThreshold = 0.5f - FLOAT_EPSILON;
            EmbeddedCoreType<ScalarType, 3, 1> result;
            ScalarType singularity = this->w() * this->y() - this->z() * this->x();

            if (singularity < -singularityThreshold)
            {
                result.z() = 2.0f * atan2(this->x(), this->w());
                result.y() = -M_PI * 0.5f;
                result.x() = 0;
            }
            else if (singularity > singularityThreshold)
            {
                result.z() = -2.0f * atan2(this->x(), this->w());
                result.y() = M_PI * 0.5f;
                result.x() = 0;
            }
            else
            {
                result.x() = atan2(2.0f * (this->w() * this->x() + this->y() * this->z()), 1.0f - 2.0f * (this->x() * this->x() + this->y() * this->y()));
                result.y() = asin(2.0f * singularity);
                result.z() = atan2(2.0f * (this->w() * this->z() + this->x() * this->y()), 1.0f - 2.0f * (this->y() * this->y() + this->z() * this->z()));
            }
            return result;
        }

        inline void setIdentity()
        {
            this->w() = (ScalarType)1;
            this->x() = (ScalarType)0;
            this->y() = (ScalarType)0;
            this->z() = (ScalarType)0;
            return;
        }

        static inline EmbeddedQuaternion Identity()
        {
            EmbeddedQuaternion result;
            result.w() = (ScalarType)1;
            return result;
        }
    };

}

namespace EmbeddedMath
{
    using namespace EmbeddedTypes;

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

}
#endif // EMBEDDEDMATH_HPP