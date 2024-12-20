// Copyright 2024 Liu Chuangye @ chuangyeliu0206@gmail.com
// Apache Licence 2.0
#ifndef EMBEDDEDMATH_HPP_
#define EMBEDDEDMATH_HPP_

#include "math.h"
#include "string.h"
#define FLOAT_EPSILON 1.1920929e-7f

namespace EmbeddedTypes
{
    template <typename Scalar, int rows, int cols>
    class EmbeddedCoreType
    {
    protected:
        Scalar Elements[rows * cols];
        static constexpr int size = rows * cols;

    public:
        using ScalarType = Scalar;
        static constexpr int RowsAtCompileTime = rows;
        static constexpr int ColsAtCompileTime = cols;

        EmbeddedCoreType()
        {
            memset(Elements, 0, sizeof(Scalar) * size);
        }

        ~EmbeddedCoreType() {}

        inline Scalar &operator()(int index)
        {
            return Elements[index];
        }

        inline const Scalar &operator()(int index) const
        {
            return Elements[index];
        }

        inline Scalar &operator()(int row, int col)
        {
            return Elements[row * cols + col];
        }

        inline const Scalar &operator()(int row, int col) const
        {
            return Elements[row * cols + col];
        }

        inline Scalar *data()
        {
            return Elements;
        }

        inline const Scalar *data() const
        {
            return Elements;
        }

        template <int length>
        inline const EmbeddedCoreType<Scalar, length, 1> head() const
        {
            EmbeddedCoreType<Scalar, length, 1> result;
            for (int i = 0; i < length; i++)
            {
                result(i) = Elements[i];
            }
            return result;
        }

        template <int length>
        inline const EmbeddedCoreType<Scalar, length, 1> tail() const
        {
            EmbeddedCoreType<Scalar, length, 1> result;
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

        inline EmbeddedCoreType operator*(const Scalar factor) const
        {
            EmbeddedCoreType result;
            for (int i = 0; i < size; i++)
            {
                result(i) = Elements[i] * factor;
            }
            return result;
        }

        friend EmbeddedCoreType operator*(Scalar factor, const EmbeddedCoreType &matrix)
        {
            return matrix * factor;
        }

        inline EmbeddedCoreType operator/(const Scalar factor) const
        {
            if (factor == 0)
                factor += 1e-10; // in case divide by zero
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

        inline EmbeddedCoreType &operator*=(const Scalar factor)
        {
            for (int i = 0; i < size; i++)
            {
                Elements[i] *= factor;
            }
            return *this;
        }

        inline EmbeddedCoreType &operator/=(const Scalar factor)
        {
            if (factor == 0)
                factor += 1e-10; // in case divide by zero
            for (int i = 0; i < size; i++)
            {
                Elements[i] /= factor;
            }
            return *this;
        }

        inline EmbeddedCoreType<Scalar, cols, rows> transpose() const
        {
            EmbeddedCoreType<Scalar, cols, rows> result;
            for (int i = 0; i < rows; i++)
            {
                for (int j = 0; j < cols; j++)
                {
                    result(j, i) = Elements[i * cols + j];
                }
            }
            return result;
        }

        inline void setZero()
        {
            memset(Elements, 0, size * sizeof(Scalar));
            return;
        }

        static inline EmbeddedCoreType Zero()
        {
            EmbeddedCoreType result;
            memset(result.data(), (Scalar)0, size * sizeof(Scalar));
            return result;
        }

        static inline EmbeddedCoreType Ones()
        {
            EmbeddedCoreType result;
            memset(result.data(), (Scalar)1, size * sizeof(Scalar));
            return result;
        }

        inline Scalar norm() const
        {
            Scalar result = 0;
            for (int i = 0; i < size; i++)
            {
                result += Elements[i] * Elements[i];
            }
            return sqrt(result);
        }

        inline EmbeddedCoreType normalized() const
        {
            EmbeddedCoreType result;
            Scalar _norm = norm();
            for (int i = 0; i < size; i++)
            {
                result(i) = Elements[i] / _norm;
            }
            return result;
        }

        inline void normalize()
        {
            Scalar _norm = norm();
            for (int i = 0; i < size; i++)
            {
                Elements[i] /= _norm;
            }
            return;
        }

        inline Scalar dot(const EmbeddedCoreType &other) const
        {
            Scalar result = 0;
            for (int i = 0; i < size; i++)
            {
                result += this->Elements[i] * other(i);
            }
            return result;
        }

        inline EmbeddedCoreType inverse() const
        {
            EmbeddedCoreType result;
            if constexpr (RowsAtCompileTime == 2 && ColsAtCompileTime == 2)
            {
                Scalar det = this->Elements[0] * this->Elements[3] - this->Elements[1] * this->Elements[2];
                if (det == 0)
                {
                    return EmbeddedCoreType::Zero();
                }
                Scalar invDet = (Scalar)1 / det;

                result(0, 0) = this->Elements[3] * invDet;
                result(0, 1) = -this->Elements[1] * invDet;
                result(1, 0) = -this->Elements[2] * invDet;
                result(1, 1) = this->Elements[0] * invDet;
            }
            else if constexpr (RowsAtCompileTime == 3 && ColsAtCompileTime == 3)
            {

                Scalar det =
                    this->Elements[0] * (this->Elements[4] * this->Elements[8] - this->Elements[5] * this->Elements[7]) - this->Elements[1] * (this->Elements[3] * this->Elements[8] - this->Elements[5] * this->Elements[6]) + this->Elements[2] * (this->Elements[3] * this->Elements[7] - this->Elements[4] * this->Elements[6]);

                if (det == 0)
                {
                    return EmbeddedCoreType::Zero();
                }
                Scalar invDet = (Scalar)1 / det;

                result(0, 0) = (this->Elements[4] * this->Elements[8] - this->Elements[5] * this->Elements[7]) * invDet;
                result(0, 1) = (this->Elements[2] * this->Elements[7] - this->Elements[1] * this->Elements[8]) * invDet;
                result(0, 2) = (this->Elements[1] * this->Elements[5] - this->Elements[2] * this->Elements[4]) * invDet;

                result(1, 0) = (this->Elements[5] * this->Elements[6] - this->Elements[3] * this->Elements[8]) * invDet;
                result(1, 1) = (this->Elements[0] * this->Elements[8] - this->Elements[2] * this->Elements[6]) * invDet;
                result(1, 2) = (this->Elements[2] * this->Elements[3] - this->Elements[0] * this->Elements[5]) * invDet;

                result(2, 0) = (this->Elements[3] * this->Elements[7] - this->Elements[4] * this->Elements[6]) * invDet;
                result(2, 1) = (this->Elements[1] * this->Elements[6] - this->Elements[0] * this->Elements[7]) * invDet;
                result(2, 2) = (this->Elements[0] * this->Elements[4] - this->Elements[1] * this->Elements[3]) * invDet;
            }
            else
            {
                // TODO : implement
            }
            return result;
        }

        //! do nothing
        inline EmbeddedCoreType eval()
        {
            return *this;
        }
    };

    template <typename Scalar, int rows, int cols, int common>
    inline EmbeddedCoreType<Scalar, rows, cols> operator*(const EmbeddedCoreType<Scalar, rows, common> &A,
                                                          const EmbeddedCoreType<Scalar, common, cols> &B)
    {
        EmbeddedCoreType<Scalar, rows, cols> result = EmbeddedCoreType<Scalar, rows, cols>::Zero();
        for (int i = 0; i < rows; i++)
        {
            for (int j = 0; j < cols; j++)
            {
                for (int k = 0; k < common; k++)
                {
                    result(i, j) += A(i, k) * B(k, j);
                }
            }
        }
        return result;
    }

    template <typename Scalar, int Dim>
    class EmbeddedMatrix : public EmbeddedCoreType<Scalar, Dim, Dim>
    {
    public:
        using BaseType = EmbeddedCoreType<Scalar, Dim, Dim>;
        // using BaseType::operator*;
        EmbeddedMatrix() : BaseType() {}
        EmbeddedMatrix(const BaseType &other)
        {
            memcpy(this->Elements, other.data(), sizeof(Scalar) * Dim * Dim);
        }

        inline float trace() const
        {
            float result = 0;
            for (int i = 0; i < Dim; i++)
            {
                result += this->Elements[i * Dim + i];
            }
            return result;
        }

        inline void setIdentity()
        {
            for (int i = 0; i < Dim; i++)
            {
                this->Elements[i * Dim + i] = (Scalar)1;
            }
            return;
        }

        static inline EmbeddedMatrix Identity()
        {
            EmbeddedMatrix result;
            for (int i = 0; i < Dim; i++)
            {
                result(i, i) = (Scalar)1;
            }
            return result;
        }

        //! TO BE CONTINUED...
    };

    template <typename Scalar>
    class EmbeddedVector2 : public EmbeddedCoreType<Scalar, 2, 1>
    {
    public:
        using BaseType = EmbeddedCoreType<Scalar, 2, 1>;

        EmbeddedVector2() : BaseType() {}
        EmbeddedVector2(Scalar x, Scalar y) : BaseType()
        {
            this->Elements[0] = x;
            this->Elements[1] = y;
        }
        EmbeddedVector2(const BaseType &other)
        {
            this->Elements[0] = other(0);
            this->Elements[1] = other(1);
        }
        inline Scalar &x() { return this->Elements[0]; }
        inline const Scalar &x() const { return this->Elements[0]; }
        inline Scalar &y() { return this->Elements[1]; }
        inline const Scalar &y() const { return this->Elements[1]; }

        inline EmbeddedMatrix<Scalar, 2> asDiagonal() const
        {
            EmbeddedMatrix<Scalar, 2> result = EmbeddedMatrix<Scalar, 2>::Zero();
            result(0, 0) = this->Elements[0];
            result(1, 1) = this->Elements[1];
            return result;
        }
    };

    template <typename Scalar>
    class EmbeddedVector3 : public EmbeddedCoreType<Scalar, 3, 1>
    {
    public:
        using BaseType = EmbeddedCoreType<Scalar, 3, 1>;

        EmbeddedVector3() : BaseType() {}
        EmbeddedVector3(Scalar x, Scalar y, Scalar z) : BaseType()
        {
            this->Elements[0] = x;
            this->Elements[1] = y;
            this->Elements[2] = z;
        }
        EmbeddedVector3(const BaseType &other)
        {
            this->Elements[0] = other(0);
            this->Elements[1] = other(1);
            this->Elements[2] = other(2);
        }
        inline Scalar &x() { return this->Elements[0]; }
        inline const Scalar &x() const { return this->Elements[0]; }
        inline Scalar &y() { return this->Elements[1]; }
        inline const Scalar &y() const { return this->Elements[1]; }
        inline Scalar &z() { return this->Elements[2]; }
        inline const Scalar &z() const { return this->Elements[2]; }

        inline EmbeddedVector3 cross(const EmbeddedVector3 &other) const
        {
            EmbeddedVector3 result;
            result(0) = this->Elements[1] * other(2) - this->Elements[2] * other(1);
            result(1) = this->Elements[2] * other(0) - this->Elements[0] * other(2);
            result(2) = this->Elements[0] * other(1) - this->Elements[1] * other(0);
            return result;
        }

        inline EmbeddedMatrix<Scalar, 3> asDiagonal() const
        {
            EmbeddedMatrix<Scalar, 3> result = EmbeddedMatrix<Scalar, 3>::Zero();
            result(0, 0) = this->Elements[0];
            result(1, 1) = this->Elements[1];
            result(2, 2) = this->Elements[2];
            return result;
        }
    };

    template <typename Scalar>
    class EmbeddedVector4 : public EmbeddedCoreType<Scalar, 4, 1>
    {
    public:
        using BaseType = EmbeddedCoreType<Scalar, 4, 1>;

        EmbeddedVector4() : BaseType() {}
        EmbeddedVector4(Scalar x, Scalar y, Scalar z, Scalar w) : BaseType()
        {
            this->Elements[0] = x;
            this->Elements[1] = y;
            this->Elements[2] = z;
            this->Elements[3] = w;
        }
        EmbeddedVector4(const BaseType &other)
        {
            this->Elements[0] = other(0);
            this->Elements[1] = other(1);
            this->Elements[2] = other(2);
            this->Elements[3] = other(3);
        }
        inline Scalar &x() { return this->Elements[0]; }
        inline const Scalar &x() const { return this->Elements[0]; }
        inline Scalar &y() { return this->Elements[1]; }
        inline const Scalar &y() const { return this->Elements[1]; }
        inline Scalar &z() { return this->Elements[2]; }
        inline const Scalar &z() const { return this->Elements[2]; }
        inline Scalar &w() { return this->Elements[3]; }
        inline const Scalar &w() const { return this->Elements[3]; }

        inline EmbeddedMatrix<Scalar, 4> asDiagonal() const
        {
            EmbeddedMatrix<Scalar, 4> result = EmbeddedMatrix<Scalar, 4>::Zero();
            result(0, 0) = this->Elements[0];
            result(1, 1) = this->Elements[1];
            result(2, 2) = this->Elements[2];
            result(3, 3) = this->Elements[3];
            return result;
        }
    };

    template <typename Scalar>
    class EmbeddedQuaternion : public EmbeddedVector4<Scalar>
    {
    public:
        using BaseType = EmbeddedCoreType<Scalar, 4, 1>;

        EmbeddedQuaternion() : EmbeddedVector4<Scalar>() {}
        EmbeddedQuaternion(Scalar x, Scalar y, Scalar z, Scalar w) : EmbeddedVector4<Scalar>(x, y, z, w) {}
        EmbeddedQuaternion(const BaseType &other)
        {
            this->Elements[0] = other(0);
            this->Elements[1] = other(1);
            this->Elements[2] = other(2);
            this->Elements[3] = other(3);
        }

        inline EmbeddedVector3<Scalar> vec() const
        {
            EmbeddedVector3<Scalar> result;
            result(0) = this->x();
            result(1) = this->y();
            result(2) = this->z();
            return result;
        }

        inline void setXYZ(const EmbeddedVector3<Scalar> &vec)
        {
            this->x() = vec(0);
            this->y() = vec(1);
            this->z() = vec(2);
            return;
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

        inline EmbeddedQuaternion operator*(const EmbeddedQuaternion &other) const
        {
            EmbeddedQuaternion result;
            result.x() = other.w() * this->x() + other.z() * this->y() - other.y() * this->z() + other.x() * this->w();
            result.y() = -other.z() * this->x() + other.w() * this->y() + other.x() * this->z() + other.y() * this->w();
            result.z() = other.y() * this->x() - other.x() * this->y() + other.w() * this->z() + other.z() * this->w();
            result.w() = -other.x() * this->x() - other.y() * this->y() - other.z() * this->z() + other.w() * this->w();
            return result;
        }

        inline void setIdentity()
        {
            this->w() = (Scalar)1;
            this->x() = (Scalar)0;
            this->y() = (Scalar)0;
            this->z() = (Scalar)0;
            return;
        }

        inline EmbeddedMatrix<Scalar, 3> toRotationMatrix() const
        {
            Scalar w = this->w();
            Scalar x = this->x();
            Scalar y = this->y();
            Scalar z = this->z();
            EmbeddedMatrix<Scalar, 3> result;
            result(0, 0) = (Scalar)1.0f - (Scalar)2.0f * (y * y + z * z);
            result(0, 1) = (Scalar)2.0f * (x * y - w * z);
            result(0, 2) = (Scalar)2.0f * (x * z + w * y);

            result(1, 0) = (Scalar)2.0f * (x * y + w * z);
            result(1, 1) = (Scalar)1.0f - (Scalar)2.0f * (x * x + z * z);
            result(1, 2) = (Scalar)2.0f * (y * z - w * x);

            result(2, 0) = (Scalar)2.0f * (x * z - w * y);
            result(2, 1) = (Scalar)2.0f * (y * z + w * x);
            result(2, 2) = (Scalar)1.0f - (Scalar)2.0f * (x * x + y * y);
            return result;
        }

        static inline EmbeddedQuaternion Identity()
        {
            EmbeddedQuaternion result;
            result.w() = (Scalar)1;
            return result;
        }
    };

}

namespace EmbeddedMath
{
    using namespace EmbeddedTypes;

    using Vector2f = EmbeddedVector2<float>;
    using Vector3f = EmbeddedVector3<float>;
    using Vector4f = EmbeddedVector4<float>;

    using Quaternionf = EmbeddedQuaternion<float>;

    using Matrix2f = EmbeddedMatrix<float, 2>;
    using Matrix3f = EmbeddedMatrix<float, 3>;
    using Matrix4f = EmbeddedMatrix<float, 4>;

    /// @brief returns skew symmetric matrix
    /// @param w
    /// @return
    inline Matrix3f skew(const Vector3f &w)
    {
        Matrix3f w_x = Matrix3f::Zero();
        w_x(0, 1) = -w(2);
        w_x(0, 2) = w(1);
        w_x(1, 0) = w(2);
        w_x(1, 2) = -w(0);
        w_x(2, 0) = -w(1);
        w_x(2, 1) = w(0);
        return w_x;
    }

    /// @brief returns vector portion of skew-symmetric
    /// @param w_x
    /// @return
    inline Vector3f vee(const Matrix3f &w_x)
    {
        Vector3f w;
        w(0) = w_x(2, 1);
        w(1) = w_x(0, 2);
        w(2) = w_x(1, 0);
        return w;
    }

    //! NOTE: this function is an estimation. When norm > 1, do not use it;
    /// @brief use 4th-order estimation for SO3 exp
    /// @param w
    /// @return
    inline Matrix3f so3_Exp(const Vector3f &w)
    {
        Matrix3f w_x = skew(w);
        float theta_sq = w.dot(w);
        float A, B;
        Matrix3f R;
        if (theta_sq < FLOAT_EPSILON * FLOAT_EPSILON)
        {
            A = 1;
            B = 0.5;
            if (theta_sq == float(0))
                R = Matrix3f::Identity();
            else
            {
                R = Matrix3f::Identity() + A * w_x + B * w_x * w_x;
            }
        }
        else
        {
            float theta_quad = theta_sq * theta_sq;
            A = 1.0f - theta_sq / 6.0f + theta_quad / 120.0f;
            B = 0.5f - theta_sq / 24.0f + theta_quad / 720.0f;
            R = Matrix3f::Identity() + A * w_x + B * w_x * w_x;
        }
        return R;
    }

    /// @brief Logarithm map of SO3
    /// @param R
    /// @return
    inline Vector3f so3_Log(const Matrix3f &R)
    {
        float a = 0.5f * (R.trace() - 1.0f);
        float theta = (a >= 1.0f) ? 0 : ((a <= -1.0f) ? M_PI : acos(a));
        float D;
        if (theta < FLOAT_EPSILON)
        {
            D = 0.5f;
        }
        else if (abs(sin(theta)) < FLOAT_EPSILON)
        {
            Vector3f vec;

            vec.x() = sqrt((R(0.0f, 0.0f) + 1.0f) * M_PI_2 * M_PI);

            vec.y() = sqrt((R(1.0f, 1.0f) + 1.0f) * M_PI_2 * M_PI);

            vec.z() = sqrt((R(2.0f, 2.0f) + 1.0f) * M_PI_2 * M_PI);
            return vec;
        }
        else
        {
            D = theta / (2.0f * sin(theta));
        }

        Matrix3f w_x = D * (R - R.transpose());

        if (R != Matrix3f::Identity())
        {
            Vector3f vec;
            vec.x() = w_x(2, 1);
            vec.y() = w_x(0, 2);
            vec.z() = w_x(1, 0);
            return vec;
        }
        else
        {
            return Vector3f::Zero();
        }
    }

    /// @brief 4th order estimation of quaternion exp
    /// @param w
    /// @return
    inline Quaternionf quat_Exp(const Vector3f &w)
    {
        float squared_norm = w.dot(w);
        Quaternionf q;
        q.setIdentity();
        if (squared_norm < FLOAT_EPSILON * FLOAT_EPSILON)
        {
            if (squared_norm = float(0))
                return q;
            else
            {
                q.w() = 1;

                q.setXYZ(0.5f * w);
                q.normalize();
                return q;
            }
        }
        q.w() = 1 - 0.125f * squared_norm + squared_norm * squared_norm / 384.0f;
        Vector3f tmp_w = (0.5f - squared_norm / (48.0f) + squared_norm * squared_norm / (3840.0f)) * w;
        q.setXYZ(tmp_w);
        q.normalize();
        return q;
    }

}
#endif // EMBEDDEDMATH_HPP