#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"
#define EIGEN_DONT_VECTORIZE
#include <EmbeddedMath.hpp>
#include <Eigen/Dense>
#include <chrono>
#include <iostream>

template <typename T, int rows, int cols>
bool isApprox(Eigen::Matrix<T, rows, cols> const &lhs, EmbeddedMath::Matrix<T, rows, cols> const &rhs)
{
    T threshold = 1e-7;
    bool isEqual = false;

    for (int i = 0; i < rows * cols; i++)
    {
        if (std::abs(lhs.data()[i] - rhs.data()[i]) > threshold)
        {
            printf("Eigen and EmbeddedMath are not equal at index %d, %f != %f\n", i, lhs.data()[i], rhs.data()[i]);
            isEqual = false;
            break;
        }
        isEqual = true;
    }
    return isEqual;
}

TEST_CASE("Test with Eigen")
{
    Eigen::Vector3d eigen_v1(1, 2, 3);
    EmbeddedMath::Vector3d embedded_v1(1, 2, 3);

    CHECK(isApprox(eigen_v1, embedded_v1));

    Eigen::Matrix3d eigen_m1;
    eigen_m1(0) = 1;
    eigen_m1(1) = 2;
    eigen_m1(2) = 3;
    eigen_m1(3) = 4;
    eigen_m1(4) = 0;
    eigen_m1(5) = 6;
    eigen_m1(6) = 0;
    eigen_m1(7) = 8;
    eigen_m1(8) = 9;

    EmbeddedMath::Matrix3d embedded_m1;
    embedded_m1(0) = 1;
    embedded_m1(1) = 2;
    embedded_m1(2) = 3;
    embedded_m1(3) = 4;
    embedded_m1(4) = 0;
    embedded_m1(5) = 6;
    embedded_m1(6) = 0;
    embedded_m1(7) = 8;
    embedded_m1(8) = 9;

    Eigen::Vector3d eigen_v2 = eigen_m1 * eigen_v1;
    EmbeddedMath::Vector3d embedded_v2 = embedded_m1 * embedded_v1;

    CHECK(isApprox(eigen_v2, embedded_v2));
    CHECK(isApprox(eigen_m1, embedded_m1));
    CHECK(isApprox(eigen_m1, embedded_m1.eval()));
    CHECK(isApprox(Eigen::Matrix3d(eigen_m1.transpose()), embedded_m1.transpose()));
    CHECK(isApprox(Eigen::Matrix3d(eigen_m1.inverse()), embedded_m1.inverse()));

    // Test Matrix multiplication

    Eigen::Matrix<double, 3, 4> eigen_m2;
    eigen_m2(0) = 1;
    eigen_m2(1) = 2;
    eigen_m2(2) = 3;
    eigen_m2(3) = 4;
    eigen_m2(4) = 5;
    eigen_m2(5) = 6;
    eigen_m2(6) = 7;
    eigen_m2(7) = 8;
    eigen_m2(8) = 9;
    eigen_m2(9) = 10;
    eigen_m2(10) = 11;
    eigen_m2(11) = 12;

    EmbeddedMath::Matrix<double, 3, 4> embedded_m2;
    embedded_m2(0) = 1;
    embedded_m2(1) = 2;
    embedded_m2(2) = 3;
    embedded_m2(3) = 4;
    embedded_m2(4) = 5;
    embedded_m2(5) = 6;
    embedded_m2(6) = 7;
    embedded_m2(7) = 8;
    embedded_m2(8) = 9;
    embedded_m2(9) = 10;
    embedded_m2(10) = 11;
    embedded_m2(11) = 12;

    Eigen::Matrix<double, 3, 4> eigen_m3 = eigen_m1 * eigen_m2;
    EmbeddedMath::Matrix<double, 3, 4> embedded_m3 = embedded_m1 * embedded_m2;
    CHECK(isApprox(eigen_m3, embedded_m3));

    Eigen::Matrix<double, 3, 4> eigen_m4 = eigen_m1 * eigen_m1 * eigen_m2;
    EmbeddedMath::Matrix<double, 3, 4> embedded_m4 = embedded_m1 * embedded_m1 * embedded_m2;
    CHECK(isApprox(eigen_m4, embedded_m4));

    // Test quaternion
    Eigen::Quaterniond eigen_q1(0.707f, 0.0f, 0.707f, 0.0f);
    EmbeddedMath::Quaterniond embedded_q1(0.707f, 0.0f, 0.707f, 0.0f);

    CHECK(isApprox(Eigen::Matrix3d(eigen_q1.toRotationMatrix()), embedded_q1.toRotationMatrix()));

    // Test quaternion multiplication
    Eigen::Quaterniond eigen_q2(0.707f, 0.0f, -0.707f, 0.0f);
    EmbeddedMath::Quaterniond embedded_q2(0.707f, 0.0f, -0.707f, 0.0f);

    Eigen::Quaterniond eigen_q3 = eigen_q1 * eigen_q2;
    EmbeddedMath::Quaterniond embedded_q3 = embedded_q1 * embedded_q2;

    CHECK(isApprox(eigen_q3.toRotationMatrix(), embedded_q3.toRotationMatrix()));

    auto eigen_euler = eigen_q3.toRotationMatrix().eulerAngles(2, 1, 0);
    auto embedded_euler = embedded_q3.toRotationMatrix().eulerAngles(2, 1, 0);
    CHECK(isApprox(eigen_euler, embedded_euler));
}

TEST_CASE("Benchmark with Eigen")
{
    Eigen::setNbThreads(1);
    EmbeddedMath::Matrix4d A = EmbeddedMath::Matrix4d::Zero();
    A(0, 0) = 1;
    A(0, 1) = 2;
    A(0, 2) = 3;
    A(0, 3) = 4;
    A(1, 0) = 2;
    A(1, 1) = 3;
    A(1, 2) = 2;
    A(1, 3) = 5;
    A(2, 0) = 1;
    A(2, 1) = 2;
    A(2, 2) = 3;
    A(2, 3) = 2;
    A(3, 0) = 1;
    A(3, 1) = -1;
    A(3, 2) = 0;
    A(3, 3) = 2;

    Eigen::Matrix4d B = Eigen::Matrix4d::Zero();
    B(0, 0) = 1;
    B(0, 1) = 2;
    B(0, 2) = 3;
    B(0, 3) = 4;
    B(1, 0) = 2;
    B(1, 1) = 3;
    B(1, 2) = 2;
    B(1, 3) = 5;
    B(2, 0) = 1;
    B(2, 1) = 2;
    B(2, 2) = 3;
    B(2, 3) = 2;
    B(3, 0) = 1;
    B(3, 1) = -1;
    B(3, 2) = 0;
    B(3, 3) = 2;

    EmbeddedMath::Matrix4d C;
    Eigen::Matrix4d D;
    auto start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < 1000000; ++i)
    {
        __asm__ volatile("nop");
        C = (A*A).eval();
        // D = (B * B).eval();
    }
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    std::cout << "Time taken by function: " << duration.count() << " microseconds" << std::endl;
}