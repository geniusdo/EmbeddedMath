#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

#include <EmbeddedMath.hpp>

template <typename T>
bool isApprox(T a, T b, T epsilon = 0.0001)
{
    return abs(a - b) <= epsilon;
}

TEST_CASE("Quaternion")
{
    using namespace EmbeddedMath;

    float threshold = 0.00001f;

    // Test default constructor
    Quaternionf q1;
    CHECK(q1(0) == 0.0f); // x
    CHECK(q1(1) == 0.0f); // y
    CHECK(q1(2) == 0.0f); // z
    CHECK(q1(3) == 0.0f); // w

    q1 = Quaternionf::Identity();
    CHECK(q1(0) == 0.0f); // x
    CHECK(q1(1) == 0.0f); // y
    CHECK(q1(2) == 0.0f); // z
    CHECK(q1(3) == 1.0f); // w

    // Test constructor with values
    Quaternionf q2(1.0f, 2.0f, 3.0f, 4.0f);
    CHECK(q2(0) == 2.0f); // x
    CHECK(q2(1) == 3.0f); // y
    CHECK(q2(2) == 4.0f); // z
    CHECK(q2(3) == 1.0f); // w

    // // Test copy constructor
    Quaternionf q3(q2);
    CHECK(q3(0) == 2.0f); // x
    CHECK(q3(1) == 3.0f); // y
    CHECK(q3(2) == 4.0f); // z
    CHECK(q3(3) == 1.0f); // w

    // Test constructor from rotation matrix
    Matrix3f mat;
    mat(0, 0) = 1.0f;
    mat(0, 1) = 0.0f;
    mat(0, 2) = 0.0f;
    mat(1, 0) = 0.0f;
    mat(1, 1) = 1.0f;
    mat(1, 2) = 0.0f;
    mat(2, 0) = 0.0f;
    mat(2, 1) = 0.0f;
    mat(2, 2) = 1.0f; // Identity matrix

    Quaternionf q4(mat);  // Should convert identity matrix to identity quaternion
    CHECK(q4(0) == 0.0f); // x
    CHECK(q4(1) == 0.0f); // y
    CHECK(q4(2) == 0.0f); // z
    CHECK(q4(3) == 1.0f); // w

    // Test quaternion multiplication
    Quaternionf q5_1(1.0f, 0.0f, 0.0f, 0.0f); // Identity quaternion
    // 90-degree rotation about x-axis
    Quaternionf q6_1(0.0f, 1.0f, 0.0f, 0.0f); // 90-degree rotation about x-axis
    Quaternionf result = q5_1 * q6_1;

    // 90-degree rotation about x-axis (q5_1 * q6_1)
    CHECK(result(0) == 1.0f); // x
    CHECK(result(1) == 0.0f); // y
    CHECK(result(2) == 0.0f); // z
    CHECK(result(3) == 0.0f); // w

    // 90-degree rotation about y-axis
    Quaternionf q5_2(0.707f, 0.0f, 0.707f, 0.0f); // 90-degree rotation about y-axis
    Quaternionf q6_2(0.707f, 0.0f, 0.0f, 0.707f); // 90-degree rotation about z-axis
    Quaternionf result2 = q5_2 * q6_2;            // q5_2 * q6_2 = rotation about y and then z

    // Result of q5_2 * q6_2 (90-degree about y, then 90-degree about z)
    CHECK(isApprox(result2(0), 0.499849f)); // x
    CHECK(isApprox(result2(1), 0.499849f)); // y
    CHECK(isApprox(result2(2), 0.499849f)); // z
    CHECK(isApprox(result2(3), 0.499849f)); // w

    // Test quaternion conjugate
    Quaternionf q7(1.0f, 2.0f, 3.0f, 4.0f);
    Quaternionf conjugate = q7.conjugate();
    CHECK(conjugate(0) == -2.0f); // x
    CHECK(conjugate(1) == -3.0f); // y
    CHECK(conjugate(2) == -4.0f); // z
    CHECK(conjugate(3) == 1.0f);  // w

    // Test quaternion inverse
    Quaternionf q8(1.0f, 2.0f, 3.0f, 4.0f);
    Quaternionf inverse = q8.inverse();
    CHECK(inverse(0) == -2.0f / q8.norm()); // x
    CHECK(inverse(1) == -3.0f / q8.norm()); // y
    CHECK(inverse(2) == -4.0f / q8.norm()); // z
    CHECK(inverse(3) == 1.0f / q8.norm());  // w

    // Test quaternion norm
    Quaternionf q9(1.0f, 2.0f, 3.0f, 4.0f);
    float norm = q9.norm();
    CHECK(norm == float(sqrt(pow(1.0f, 2) + pow(2.0f, 2) + pow(3.0f, 2) + pow(4.0f, 2))));

    // Test quaternion normalization
    Quaternionf q10(1.0f, 2.0f, 3.0f, 4.0f);
    q10.normalize();
    float normAfter = q10.norm();
    CHECK(isApprox(normAfter, 1.0f)); // Should be normalized to unit length

    // Test quaternion and rotation matrix conversion
    Quaternionf q11 = Quaternionf::Identity();
    Matrix3f mat2 = q11.toRotationMatrix();
    CHECK(mat2(0, 0) == 1.0f); // x
    CHECK(mat2(1, 1) == 1.0f); // y
    CHECK(mat2(2, 2) == 1.0f); // z
    
    Quaternionf q12(mat2);
    CHECK(q12(0) == 0.0f); // x
    CHECK(q12(1) == 0.0f); // y
    CHECK(q12(2) == 0.0f); // z
    CHECK(q12(3) == 1.0f); // w
}