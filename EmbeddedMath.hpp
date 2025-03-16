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
    class EmbeddedCoreType;

    template <typename ScalarType, int rows, int cols>
    class EmbeddedRefType;

    template <typename ScalarType>
    class EmbeddedQuaternion;

    template <class MatrixType>
    class PartialPivLU;

    template <typename ScalarType, int rows, int cols>
    class EmbeddedRefType
    {
    protected:
        ScalarType (&RefElements)[rows * cols];
        static constexpr int size = rows * cols;
        const int subRows, subCols, subSize, startRow, startCol;
        bool isColumnMajorOrder;

    public:
        using Scalar = ScalarType;

        EmbeddedRefType() = delete;

        EmbeddedRefType(ScalarType (&Elements)[rows * cols],
                        const int sub_rows, const int sub_cols,
                        const int start_row, const int start_col) : RefElements(Elements),
                                                                    subRows(sub_rows),
                                                                    subCols(sub_cols),
                                                                    subSize(sub_rows * sub_cols),
                                                                    startRow(start_row),
                                                                    startCol(start_col)
        {
            if ((sub_rows == rows) || (sub_cols == 1))
                isColumnMajorOrder = true;
            else
                isColumnMajorOrder = false;
        }

        inline ScalarType &operator()(int index)
        {
            int refIndex = ((int)(index / subRows) + startCol) * rows + (index % subRows) + startRow;
            return RefElements[refIndex];
        }

        inline const ScalarType &operator()(int index) const
        {
            int refIndex = ((int)(index / subRows) + startCol) * rows + (index % subRows) + startRow;
            return RefElements[refIndex];
        }

        inline ScalarType &operator()(int row, int col)
        {
            int refIndex = (col + startCol) * rows + row + startRow;
            return RefElements[refIndex];
        }

        inline const ScalarType &operator()(int row, int col) const
        {
            int refIndex = (col + startCol) * rows + row + startRow;
            return RefElements[refIndex];
        }

        template <int SubRows, int SubCols>
        inline void operator=(const EmbeddedCoreType<Scalar, SubRows, SubCols> &other)
        {
            for (int i = 0; i < subSize; ++i)
            {
                this->operator()(i) = other(i);
            }
            return;
        }

        inline void operator=(const EmbeddedRefType<Scalar, rows, cols> &other)
        {
            for (int i = 0; i < subSize; ++i)
            {
                this->operator()(i) = other(i);
            }
            return;
        }

        inline void swap(EmbeddedRefType other)
        {
            if (this->isColumnMajorOrder = true && other.isColumnMajorOrder == true)
            {
                ScalarType tmp[subRows * subCols];
                memcpy(tmp, &(this->operator()(0)), sizeof(ScalarType) * subRows * subCols);
                memcpy(&(this->operator()(0)), &(other(0)), sizeof(ScalarType) * subRows * subCols);
                memcpy(&(other(0)), tmp, sizeof(ScalarType) * subRows * subCols);
            }
            else
            {
                for (int i = 0; i < subSize; ++i)
                {
                    ScalarType tmp = this->operator()(i);
                    this->operator()(i) = other(i);
                    other(i) = tmp;
                }
            }
            return;
        }
    };

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
            static_assert((this->RowsAtCompileTime == other.RowsAtCompileTime) && (this->ColsAtCompileTime == other.ColsAtCompileTime));
            memcpy(this->Elements, other.data(), sizeof(ScalarType) * size);
        }

        template <int RefRows, int RefCols>
        EmbeddedCoreType(const EmbeddedRefType<ScalarType, RefRows, RefCols> &other)
        {
            for (int i = 0; i < size; ++i)
            {
                this->operator()(i) = other(i);
            }
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

        inline ScalarType determinant() const
        {
            ScalarType result = 0;
            if constexpr (RowsAtCompileTime == 1 && ColsAtCompileTime == 1)
            {
                result = this->Elements[0];
            }
            else if constexpr (RowsAtCompileTime == 2 && ColsAtCompileTime == 2)
            {
                result = this->Elements[0] * this->Elements[3] - this->Elements[1] * this->Elements[2];
            }
            else if constexpr (RowsAtCompileTime == 3 && ColsAtCompileTime == 3)
            {
                result = this->Elements[0] * this->Elements[4] * this->Elements[8] -
                         this->Elements[0] * this->Elements[7] * this->Elements[5] -
                         this->Elements[3] * this->Elements[1] * this->Elements[8] +
                         this->Elements[3] * this->Elements[7] * this->Elements[2] +
                         this->Elements[6] * this->Elements[1] * this->Elements[5] -
                         this->Elements[6] * this->Elements[4] * this->Elements[2];
            }
            else
            {
                PartialPivLU<EmbeddedCoreType> lu(*this);
                result = lu.determinant();
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

        static inline EmbeddedCoreType UnitX()
        {
            static_assert(RowsAtCompileTime >= 3 && ColsAtCompileTime == 1, "UnitX() only works for vectors longer than 3x1");
            EmbeddedCoreType result;
            result.x() = static_cast<ScalarType>(1.0);
            return result;
        }

        static inline EmbeddedCoreType UnitY()
        {
            static_assert(RowsAtCompileTime >= 3 && ColsAtCompileTime == 1, "UnitY() only works for vectors longer than 3x1");
            EmbeddedCoreType result;
            result.y() = static_cast<ScalarType>(1.0);
            return result;
        }

        static inline EmbeddedCoreType UnitZ()
        {
            static_assert(RowsAtCompileTime >= 3 && ColsAtCompileTime == 1, "UnitZ() only works for vectors longer than 3x1");
            EmbeddedCoreType result;
            result.z() = static_cast<ScalarType>(1.0);
            return result;
        }

        template <int subrows, int subcols>
        inline EmbeddedRefType<Scalar, RowsAtCompileTime, ColsAtCompileTime> block(int startrows, int startcols)
        {
            return EmbeddedRefType<Scalar, RowsAtCompileTime, ColsAtCompileTime>(
                *reinterpret_cast<Scalar(*)[RowsAtCompileTime * ColsAtCompileTime]>(this->data()),
                subrows, subcols, startrows, startcols);
        }

        inline EmbeddedRefType<Scalar, RowsAtCompileTime, ColsAtCompileTime> block(int startrows, int startcols, int subrows, int subcols)
        {
            return EmbeddedRefType<Scalar, RowsAtCompileTime, ColsAtCompileTime>(
                *reinterpret_cast<Scalar(*)[RowsAtCompileTime * ColsAtCompileTime]>(this->data()),
                subrows, subcols, startrows, startcols);
        }

        inline EmbeddedRefType<Scalar, RowsAtCompileTime, ColsAtCompileTime> row(const int index)
        {
            return EmbeddedRefType<Scalar, RowsAtCompileTime, ColsAtCompileTime>(
                *reinterpret_cast<Scalar(*)[RowsAtCompileTime * ColsAtCompileTime]>(this->data()),
                1, ColsAtCompileTime, index, 0);
        }

        inline EmbeddedRefType<Scalar, RowsAtCompileTime, ColsAtCompileTime> col(const int index)
        {
            return EmbeddedRefType<Scalar, RowsAtCompileTime, ColsAtCompileTime>(
                *reinterpret_cast<Scalar(*)[RowsAtCompileTime * ColsAtCompileTime]>(this->data()),
                RowsAtCompileTime, 1, 0, index);
        }

        inline EmbeddedCoreType inverse() const
        {
            EmbeddedCoreType result;

            if constexpr (RowsAtCompileTime == 2 && ColsAtCompileTime == 2)
            {
                ScalarType det = this->determinant();
                if (abs(det) < FLOAT_EPSILON)
                    return EmbeddedCoreType::Zero();
                ScalarType invDet = (ScalarType)1.0 / det;
                result(0, 0) = this->Elements[3] * invDet;
                result(0, 1) = -this->Elements[2] * invDet;
                result(1, 0) = -this->Elements[1] * invDet;
                result(1, 1) = this->Elements[0] * invDet;
            }
            else if constexpr (RowsAtCompileTime == 3 && ColsAtCompileTime == 3)
            {
                ScalarType det = this->determinant();
                if (abs(det) < FLOAT_EPSILON)
                    return EmbeddedCoreType::Zero();
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
            else
            {
                PartialPivLU<EmbeddedCoreType> lu(*this);
                result = lu.inverse();
            }
            return result;
        }

        inline EmbeddedCoreType<ScalarType, 3, 1> eulerAngles(const int y = 2, const int p = 1, const int r = 0) const
        {
            // TODO: fix the first element to -PI/2 to PI/2
            static_assert(RowsAtCompileTime == 3 && ColsAtCompileTime == 3);
            EmbeddedCoreType<ScalarType, 3, 1> result;

            result(y) = atan2(this->Elements[5], this->Elements[8]);
            result(p) = atan2(-this->Elements[2], sqrt(this->Elements[5] * this->Elements[5] + this->Elements[8] * this->Elements[8]));
            result(r) = atan2(this->Elements[1], this->Elements[0]);
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
        if constexpr (R1 == 1 && C1_R2 == 1 && C2 == 1)
        {
            result(0) = lhs(0) * rhs(0);
        }
        else if constexpr (R1 == 2 && C1_R2 == 2 && C2 == 2)
        {
            __builtin_prefetch(&rhs(0), 0, 0);
            result(0) = lhs(0) * rhs(0) + lhs(2) * rhs(1);
            result(1) = lhs(1) * rhs(0) + lhs(3) * rhs(1);
            result(2) = lhs(0) * rhs(2) + lhs(2) * rhs(3);
            result(3) = lhs(1) * rhs(2) + lhs(3) * rhs(3);
        }
        else if constexpr (R1 == 3 && C1_R2 == 3 && C2 == 3)
        {
            __builtin_prefetch(&rhs(0), 0, 0);
            result(0) = lhs(0) * rhs(0) + lhs(3) * rhs(1) + lhs(6) * rhs(2);
            result(1) = lhs(1) * rhs(0) + lhs(4) * rhs(1) + lhs(7) * rhs(2);
            result(2) = lhs(2) * rhs(0) + lhs(5) * rhs(1) + lhs(8) * rhs(2);
            result(3) = lhs(0) * rhs(3) + lhs(3) * rhs(4) + lhs(6) * rhs(5);
            result(4) = lhs(1) * rhs(3) + lhs(4) * rhs(4) + lhs(7) * rhs(5);
            result(5) = lhs(2) * rhs(3) + lhs(5) * rhs(4) + lhs(8) * rhs(5);
            result(6) = lhs(0) * rhs(6) + lhs(3) * rhs(7) + lhs(6) * rhs(8);
            result(7) = lhs(1) * rhs(6) + lhs(4) * rhs(7) + lhs(7) * rhs(8);
            result(8) = lhs(2) * rhs(6) + lhs(5) * rhs(7) + lhs(8) * rhs(8);
        }
        else if constexpr (R1 == 4 && C1_R2 == 4 && C2 == 4)
        {
            __builtin_prefetch(&rhs(0), 0, 0);
            result(0) = lhs(0) * rhs(0) + lhs(4) * rhs(1) + lhs(8) * rhs(2) + lhs(12) * rhs(3);
            result(1) = lhs(1) * rhs(0) + lhs(5) * rhs(1) + lhs(9) * rhs(2) + lhs(13) * rhs(3);
            result(2) = lhs(2) * rhs(0) + lhs(6) * rhs(1) + lhs(10) * rhs(2) + lhs(14) * rhs(3);
            result(3) = lhs(3) * rhs(0) + lhs(7) * rhs(1) + lhs(11) * rhs(2) + lhs(15) * rhs(3);
            __builtin_prefetch(&rhs(4), 0, 0);
            result(4) = lhs(0) * rhs(4) + lhs(4) * rhs(5) + lhs(8) * rhs(6) + lhs(12) * rhs(7);
            result(5) = lhs(1) * rhs(4) + lhs(5) * rhs(5) + lhs(9) * rhs(6) + lhs(13) * rhs(7);
            result(6) = lhs(2) * rhs(4) + lhs(6) * rhs(5) + lhs(10) * rhs(6) + lhs(14) * rhs(7);
            result(7) = lhs(3) * rhs(4) + lhs(7) * rhs(5) + lhs(11) * rhs(6) + lhs(15) * rhs(7);
            __builtin_prefetch(&rhs(8), 0, 0);
            result(8) = lhs(0) * rhs(8) + lhs(4) * rhs(9) + lhs(8) * rhs(10) + lhs(12) * rhs(11);
            result(9) = lhs(1) * rhs(8) + lhs(5) * rhs(9) + lhs(9) * rhs(10) + lhs(13) * rhs(11);
            result(10) = lhs(2) * rhs(8) + lhs(6) * rhs(9) + lhs(10) * rhs(10) + lhs(14) * rhs(11);
            result(11) = lhs(3) * rhs(8) + lhs(7) * rhs(9) + lhs(11) * rhs(10) + lhs(15) * rhs(11);
            __builtin_prefetch(&rhs(12), 0, 0);
            result(12) = lhs(0) * rhs(12) + lhs(4) * rhs(13) + lhs(8) * rhs(14) + lhs(12) * rhs(15);
            result(13) = lhs(1) * rhs(12) + lhs(5) * rhs(13) + lhs(9) * rhs(14) + lhs(13) * rhs(15);
            result(14) = lhs(2) * rhs(12) + lhs(6) * rhs(13) + lhs(10) * rhs(14) + lhs(14) * rhs(15);
            result(15) = lhs(3) * rhs(12) + lhs(7) * rhs(13) + lhs(11) * rhs(14) + lhs(15) * rhs(15);
        }
        else
        {
            T sum;
            for (int i = 0; i < R1; ++i)
            {
                for (int j = 0; j < C2; ++j)
                {

                    sum = 0;
                    int k = 0;
                    __builtin_prefetch(&rhs(k, j), 0, 0);
                    for (; k < C1_R2 - 1; k += 2)
                    {
                        sum += lhs(i, k) * rhs(k, j);
                        sum += lhs(i, k + 1) * rhs(k + 1, j);
                    }
                    if constexpr ((C1_R2 % 2) == 1)
                        sum += lhs(i, k) * rhs(k, j);
                    result(i, j) = sum;
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

    template <class MatrixType>
    class PartialPivLU
    {
    protected:
        using ScalarType = typename MatrixType::Scalar;
        MatrixType L, U, P; // L is lower triangular, U is upper triangular, P is permutation matrix
        ScalarType Q[MatrixType::ColsAtCompileTime];

    public:
        PartialPivLU(const MatrixType &matrix)
        {
            //! currently only support square matrix
            static_assert(MatrixType::RowsAtCompileTime == MatrixType::ColsAtCompileTime, "only support square matrix");
            this->L;
            this->U = matrix;
            // initialize Q
            for (int i = 0; i < MatrixType::RowsAtCompileTime; ++i)
            {
                Q[i] = i;
            }
            decompose(this->U);
        }

        MatrixType matrixL()
        {
            return this->L;
        }

        MatrixType matrixU()
        {
            return this->U;
        }

        ScalarType determinant() const
        {
            ScalarType det = 1.0;
            for (int i = 0; i < MatrixType::RowsAtCompileTime; ++i)
            {
                det *= this->L(i, i);
            }
            return det;
        }

        MatrixType inverse()
        {
            MatrixType invL;
            MatrixType invU;
            MatrixType result;

            invL(0) = 1.0 / this->L(0);
            for (int i = 1; i < MatrixType::RowsAtCompileTime; ++i)
            {
                invL(i, i) = 1.0 / this->L(i, i);
                for (int j = 0; j < i; ++j)
                {
                    for (int k = j; k < i; ++k)
                        invL(i, j) -= this->L(i, k) * invL(k, j) * invL(i, i);
                }
            }

            for (int i = MatrixType::RowsAtCompileTime - 1; i >= 0; --i)
            {
                invU(i, i) = 1.0;
                for (int j = i + 1; j < MatrixType::ColsAtCompileTime; ++j)
                {
                    for (int k = i + 1; k < j; ++k)
                        invU(i, j) -= this->U(i, k) * invU(k, j);
                    invU(i, j) -= this->U(i, j);
                }
            }

            result = invU * invL;

            // swap rows according to P
            for (int i = MatrixType::RowsAtCompileTime - 1; i >= 0; --i)
            {
                if (Q[i] != i)
                    result.row(i).swap(result.row(Q[i]));
            }
            return result;
        }

    private:
        void decompose(MatrixType &matrix)
        {
            for (int k = 0; k < MatrixType::ColsAtCompileTime; ++k)
            {
                // find pivoting column index
                int pivotIndex = k;
                for (int j = k + 1; j < MatrixType::ColsAtCompileTime; ++j)
                {
                    if (fabs(matrix(k, j)) > fabs(matrix(k, pivotIndex)))
                    {
                        pivotIndex = j;
                    }
                }

                if (pivotIndex != k)
                {
                    matrix.col(k).swap(matrix.col(pivotIndex));
                    Q[k] = Q[pivotIndex];
                }

                this->L(k, k) = 1.0;
                this->L.block(k, k, MatrixType::ColsAtCompileTime - k, 1) = matrix.block(k, k, MatrixType::ColsAtCompileTime - k, 1);
                ScalarType invPivot = 1.0 / matrix(k, k);
                for (int i = k + 1; i < MatrixType::ColsAtCompileTime; ++i)
                {
                    for (int j = k + 1; j < MatrixType::RowsAtCompileTime; ++j)
                    {
                        matrix(j, i) -= matrix(k, i) * matrix(j, k) * invPivot;
                    }
                    matrix(k, i) *= invPivot;
                }
            }

            for (int k = 0; k < MatrixType::ColsAtCompileTime; ++k)
            {
                for (int i = k; i < MatrixType::RowsAtCompileTime; ++i)
                {
                    if (i == k)
                        matrix(i, k) = 1;
                    else
                        matrix(i, k) = 0;
                }
            }
            return;
        }
    };

    template <typename ScalarType>
    static inline EmbeddedQuaternion<ScalarType> AngleAxis(const ScalarType &angle, const EmbeddedCoreType<ScalarType, 3, 1> &axis)
    {
        EmbeddedQuaternion<ScalarType> result;
        result.w() = cos(angle * 0.5);
        result.vec() = sin(angle * 0.5) * axis;
        return result;
    }
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

    constexpr auto AngleAxisf = AngleAxis<float>;
    constexpr auto AngleAxisd = AngleAxis<double>;

}
#endif // EMBEDDEDMATH_HPP