#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

#include <EmbeddedMath.hpp>

template <typename T>
bool isApprox(T a, T b, T epsilon = 0.0001)
{
    return abs(a - b) <= epsilon;
}

TEST_CASE("test Vector")
{
    using namespace EmbeddedMath;

    float threshold = 0.0001f;

    // Test 1: Default constructor
    Vector2f v1;
    CHECK(v1(0) == 0.0f); // Default value should be 0
    CHECK(v1(1) == 0.0f); // Default value should be 0

    // Test 2: Scalar constructor
    Vector2f v2(5.0f); // Initialize with value 5
    CHECK(v2(0) == 5.0f);
    CHECK(v2(1) == 5.0f);

    // Test 3: Copy constructor
    Vector2f v3 = v2; // Copy v2 into v3
    CHECK(v3(0) == 5.0f);
    CHECK(v3(1) == 5.0f);

    // Test 4: Accessor methods (x, y, z)
    CHECK(v2.x() == 5.0f); // Access x should return 5.0
    CHECK(v2.y() == 5.0f); // Access y should return 5.0

    // Test 5: Add operation (v2 + v2)
    Vector2f v4 = v2 + v2;
    CHECK(v4(0) == 10.0f); // 5.0 + 5.0 = 10.0
    CHECK(v4(1) == 10.0f); // 5.0 + 5.0 = 10.0

    // Test 6: Subtract operation (v2 - v1)
    Vector2f v5 = v2 - v1;
    CHECK(v5(0) == 5.0f); // 5.0 - 0.0 = 5.0
    CHECK(v5(1) == 5.0f); // 5.0 - 0.0 = 5.0

    // Test 7: Scalar multiplication (v2 * 2.0f)
    Vector2f v6 = v2 * 2.0f;
    CHECK(v6(0) == 10.0f); // 5.0 * 2.0 = 10.0
    CHECK(v6(1) == 10.0f); // 5.0 * 2.0 = 10.0

    // Test 8: Scalar division (v6 / 2.0f)
    Vector2f v7 = v6 / 2.0f;
    CHECK(v7(0) == 5.0f); // 10.0 / 2.0 = 5.0
    CHECK(v7(1) == 5.0f); // 10.0 / 2.0 = 5.0

    // Test 9: In-place addition (v2 += v2)
    v2 += v2;              // v2 becomes v2 + v2
    CHECK(v2(0) == 10.0f); // 5.0 + 5.0 = 10.0
    CHECK(v2(1) == 10.0f); // 5.0 + 5.0 = 10.0

    // Test 10: In-place subtraction (v2 -= v1)
    v2 -= v1;              // v2 becomes v2 - v1
    CHECK(v2(0) == 10.0f); // 10.0 - 0.0 = 10.0
    CHECK(v2(1) == 10.0f); // 10.0 - 0.0 = 10.0

    // Test 11: Transpose operation for 2x2 matrix
    Matrix2f m1;
    m1(0, 0) = 1.0f;
    m1(0, 1) = 2.0f;
    m1(1, 0) = 3.0f;
    m1(1, 1) = 4.0f;
    Matrix2f m2 = m1.transpose();

    CHECK(m2(0, 0) == 1.0f); // Transpose swaps (0, 1) and (1, 0)
    CHECK(m2(0, 1) == 3.0f);
    CHECK(m2(1, 0) == 2.0f);
    CHECK(m2(1, 1) == 4.0f);

    // Test 12: Norm of a vector
    Vector3f v8;
    v8(0) = 3.0f;
    v8(1) = 4.0f;
    v8(2) = 12.0f;

    CHECK(isApprox(v8.norm(), 13.0f, threshold)); // sqrt(3^2 + 4^2 + 12^2) = 13.0

    // Test 13: Normalization of a vector
    Vector3f v9 = v8.normalized();

    CHECK(isApprox(v9.norm(), 1.0f, threshold)); // The norm of a normalized vector should be 1.0

    // Test 14: Inverse of a 2x2 matrix
    Matrix2f m3;
    m3(0, 0) = 4.0f;
    m3(0, 1) = 3.0f;
    m3(1, 0) = 2.0f;
    m3(1, 1) = 1.0f;

    Matrix2f m4 = m3.inverse();
    CHECK(m4(0, 0) == -0.5f); // Inverse of 2x2 matrix should match these values
    CHECK(m4(0, 1) == 1.5f);
    CHECK(m4(1, 0) == 1.0f);
    CHECK(m4(1, 1) == -2.0f);

    // Test 15: Identity matrix creation
    Matrix3f m5 = Matrix3f::Identity();
    CHECK(m5(0, 0) == 1.0f);
    CHECK(m5(1, 1) == 1.0f);
    CHECK(m5(2, 2) == 1.0f);
    CHECK(m5(0, 1) == 0.0f);
    CHECK(m5(0, 2) == 0.0f);
    CHECK(m5(1, 0) == 0.0f);
    CHECK(m5(1, 2) == 0.0f);
    CHECK(m5(2, 0) == 0.0f);
    CHECK(m5(2, 1) == 0.0f);

    // Test 16: Equality operator
    Matrix2f m6;
    m6(0, 0) = 4.0f;
    m6(0, 1) = 3.0f;
    m6(1, 0) = 2.0f;
    m6(1, 1) = 1.0f;

    CHECK(m3 == m6); // m3 and m6 should be equal

    // Test 17: Dot product
    Vector3f v10;
    v10(0) = 1.0f;
    v10(1) = 2.0f;
    v10(2) = 3.0f;

    Vector3f v11;
    v11(0) = 4.0f;
    v11(1) = 5.0f;
    v11(2) = 6.0f;

    CHECK(isApprox(v10.dot(v11), 32.0f, threshold)); // Dot product = 1*4 + 2*5 + 3*6 = 32.0

    // Test 18: SetZero function
    Vector2f v12(3.0f);
    v12.setZero();
    CHECK(v12(0) == 0.0f); // After setZero, all elements should be 0
    CHECK(v12(1) == 0.0f);

    // Test 19: Ones function
    Vector2f v13 = Vector2f::Ones();
    CHECK(v13(0) == 1.0f); // Ones should initialize all elements to 1
    CHECK(v13(1) == 1.0f);

    // Test 20: Test head function
    Vector3f v14;
    v14(0) = 1.0f;
    v14(1) = 2.0f;
    v14(2) = 3.0f;

    Vector2f v15 = v14.head<2>();
    CHECK(v15(0) == 1.0f); // head should return the first n elements
    CHECK(v15(1) == 2.0f);

    // Test 21: Test tail function
    Vector2f v16 = v14.tail<2>();
    CHECK(v16(0) == 2.0f); // tail should return the last n elements
    CHECK(v16(1) == 3.0f);

    // Test 22: Test cross product
    Vector3f v17;
    v17(0) = 1.0f;
    v17(1) = 2.0f;
    v17(2) = 3.0f;

    Vector3f v18;
    v18(0) = 4.0f;
    v18(1) = 5.0f;
    v18(2) = 6.0f;

    CHECK(v17.cross(v18).isApprox(Vector3f(-3.0f, 6.0f, -3.0f), threshold)); // Cross product should match these values
}

TEST_CASE("test Matrix3f")
{
    using namespace EmbeddedMath;

    float threshold = 0.0001f;

    // Create and initialize Matrix3f mat1 (invertible matrix)
    Matrix3f mat1;
    mat1(0, 0) = 1.0f;
    mat1(0, 1) = 2.0f;
    mat1(0, 2) = 3.0f;
    mat1(1, 0) = 0.0f;
    mat1(1, 1) = 4.0f;
    mat1(1, 2) = 5.0f;
    mat1(2, 0) = 0.0f;
    mat1(2, 1) = 0.0f;
    mat1(2, 2) = 6.0f;

    // Create and initialize Matrix3f mat2
    Matrix3f mat2;
    mat2(0, 0) = 9.0f;
    mat2(0, 1) = 8.0f;
    mat2(0, 2) = 7.0f;
    mat2(1, 0) = 6.0f;
    mat2(1, 1) = 5.0f;
    mat2(1, 2) = 4.0f;
    mat2(2, 0) = 3.0f;
    mat2(2, 1) = 2.0f;
    mat2(2, 2) = 1.0f;

    // Create and initialize Matrix3f mat3 (identity matrix)
    Matrix3f mat3;
    mat3(0, 0) = 1.0f;
    mat3(0, 1) = 0.0f;
    mat3(0, 2) = 0.0f;
    mat3(1, 0) = 0.0f;
    mat3(1, 1) = 1.0f;
    mat3(1, 2) = 0.0f;
    mat3(2, 0) = 0.0f;
    mat3(2, 1) = 0.0f;
    mat3(2, 2) = 1.0f; // Identity matrix

    // ------------------------------ Matrix Multiplication ------------------------------
    {
        Matrix3f result = mat1 * mat2;

        // Check the multiplication result
        CHECK(result(0, 0) == 30.0f);
        CHECK(result(0, 1) == 24.0f);
        CHECK(result(0, 2) == 18.0f);

        CHECK(result(1, 0) == 39.0f);
        CHECK(result(1, 1) == 30.0f);
        CHECK(result(1, 2) == 21.0f);

        CHECK(result(2, 0) == 18.0f);
        CHECK(result(2, 1) == 12.0f);
        CHECK(result(2, 2) == 6.0f);
    }


    // ------------------------------ Consecutive Matrix Multiplication ------------------------------
    {
        // (mat1 * mat2) * mat3
        Matrix3f result1 = mat1 * mat2;
        Matrix3f result2 = result1 * mat3;
        CHECK(result2.isApprox(mat1 * mat2 * mat3, threshold));
    }

    {
        // mat1 * (mat2 * mat3)
        Matrix3f result1 = mat2 * mat3;
        Matrix3f result2 = mat1 * result1;
        CHECK(result2.isApprox(mat1 * mat2 * mat3, threshold));
    }

    // ------------------------------ Inverse of the Matrix ------------------------------
    {
        Matrix3f inverseMat = mat1.inverse();

        Matrix3f identity = mat1 * inverseMat;

        // Check the identity matrix after multiplication with inverse
        CHECK(isApprox(identity(0, 0), 1.0f));
        CHECK(isApprox(identity(1, 1), 1.0f));
        CHECK(isApprox(identity(2, 2), 1.0f));
        CHECK(isApprox(identity(0, 1), 0.0f));
        CHECK(isApprox(identity(0, 2), 0.0f));
        CHECK(isApprox(identity(1, 0), 0.0f));
        CHECK(isApprox(identity(1, 2), 0.0f));
        CHECK(isApprox(identity(2, 0), 0.0f));
        CHECK(isApprox(identity(2, 1), 0.0f));
    }

    // ------------------------------ Matrix Transpose ------------------------------
    {
        Matrix3f transposeMat = mat1.transpose();
        CHECK(transposeMat(0, 1) == mat1(1, 0));
        CHECK(transposeMat(0, 2) == mat1(2, 0));
        CHECK(transposeMat(1, 0) == mat1(0, 1));
        CHECK(transposeMat(1, 2) == mat1(2, 1));
        CHECK(transposeMat(2, 0) == mat1(0, 2));
        CHECK(transposeMat(2, 1) == mat1(1, 2));
    }

    // ------------------------------ Matrix Evaluation ------------------------------
    {
        Matrix3f evalMat = mat1.eval();
        CHECK(evalMat == mat1); // Ensure the evaluation returns the correct matrix
    }

    // block function test
    {
        Matrix3f idMat = Matrix3f::Identity();
        auto refMat = idMat.block<2,2>(0,0);

        CHECK(refMat(0) == 1.0f);
        CHECK(refMat(1) == 0.0f);
        CHECK(refMat(2) == 0.0f);
        CHECK(refMat(3) == 1.0f);

        refMat(1) = 3.0f;
        CHECK(refMat(1) == 3.0f);
        CHECK(idMat(1) == 3.0f);

        idMat.block<2,2>(1,1) = Matrix2f::Zero();
        CHECK(idMat(2,2) == 0.0f);

        Matrix2f subMat = idMat.block<2,2>(0,0);

        idMat = Matrix3f::Identity();
        Matrix3f zeroMat = Matrix3f::Zero();

        idMat.block<2,2>(0,0).swap(zeroMat.block<2,2>(0,0));
        CHECK(zeroMat(0,0) == 1.0f);
        CHECK(idMat(0,1) == 0.0f);

    }
}

TEST_CASE("Matrix4f")
{
    using namespace EmbeddedMath;

    float threshold = 0.0001f;

    // Create and initialize Matrix4f mat1 (invertible matrix)
    Matrix4f mat1;
    mat1(0, 0) = 4.0f;
    mat1(0, 1) = 3.0f;
    mat1(0, 2) = 2.0f;
    mat1(0, 3) = 1.0f;
    mat1(1, 0) = 3.0f;
    mat1(1, 1) = 2.0f;
    mat1(1, 2) = 1.0f;
    mat1(1, 3) = 0.0f;
    mat1(2, 0) = 2.0f;
    mat1(2, 1) = 1.0f;
    mat1(2, 2) = 4.0f;
    mat1(2, 3) = 3.0f;
    mat1(3, 0) = 1.0f;
    mat1(3, 1) = 0.0f;
    mat1(3, 2) = 3.0f;
    mat1(3, 3) = 4.0f;

    // Create and initialize Matrix4f mat2
    Matrix4f mat2;
    mat2(0, 0) = 1.0f;
    mat2(0, 1) = 2.0f;
    mat2(0, 2) = 3.0f;
    mat2(0, 3) = 4.0f;
    mat2(1, 0) = 5.0f;
    mat2(1, 1) = 6.0f;
    mat2(1, 2) = 7.0f;
    mat2(1, 3) = 8.0f;
    mat2(2, 0) = 9.0f;
    mat2(2, 1) = 10.0f;
    mat2(2, 2) = 11.0f;
    mat2(2, 3) = 12.0f;
    mat2(3, 0) = 13.0f;
    mat2(3, 1) = 14.0f;
    mat2(3, 2) = 15.0f;
    mat2(3, 3) = 16.0f;

    // ------------------------------ Matrix Multiplication ------------------------------
    {
        Matrix4f result = mat1 * mat2;
        
        CHECK(result(0, 0) == 50.0f);
        CHECK(result(0, 1) == 60.0f);
        CHECK(result(0, 2) == 70.0f);
        CHECK(result(0, 3) == 80.0f);

        CHECK(result(1, 0) == 22.0f);
        CHECK(result(1, 1) == 28.0f);
        CHECK(result(1, 2) == 34.0f);
        CHECK(result(1, 3) == 40.0f);

        CHECK(result(2, 0) == 82.0f);
        CHECK(result(2, 1) == 92.0f);
        CHECK(result(2, 2) == 102.0f);
        CHECK(result(2, 3) == 112.0f);

        CHECK(result(3, 0) == 80.0f);
        CHECK(result(3, 1) == 88.0f);
        CHECK(result(3, 2) == 96.0f);
        CHECK(result(3, 3) == 104.0f);
    }

    // Add other tests like Inverse, Transpose, etc.
}