#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"
#define EIGEN_DONT_VECTORIZE
#include <EmbeddedMath.hpp>
#include <Eigen/Dense>
#include <chrono>
#include <iostream>

TEST_CASE("Benchmark with Eigen")
{
    // Eigen::setNbThreads(1);
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
    //start multiplication
    for (int i = 0; i < 1000000; ++i)
    {
        __asm__ volatile("nop");
        C = (A*A).eval();
        // D = B.inverse();
        // D = (B * B).eval();
    }
    //end multiplication
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    std::cout << "Time taken by function: " << duration.count() << " microseconds" << std::endl;
}