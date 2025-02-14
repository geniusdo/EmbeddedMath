#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

#include <EmbeddedMath.hpp>
#include <iostream>
TEST_CASE("test decomposition")
{

    using namespace EmbeddedMath;

    Matrix2d A = Matrix2d::Zero();

    A(0,0) = 0;
    A(0,1) = 1;
    A(1,0) = 2;
    A(1,1) = 1;

    PartialPivLU<Matrix2d> lu(A);
    auto L = lu.matrixL();
    printf("%matrix L\n", L(0,0), L(0,1));
    printf("%f %f\n", L(0,0), L(0,1));
    printf("%f %f\n", L(1,0), L(1,1));

    auto U = lu.matrixU();
    printf("matrix U\n");
    printf("%f %f\n", U(0,0), U(0,1));
    printf("%f %f\n", U(1,0), U(1,1));

    Matrix3f B = Matrix3f::Zero();
    B(0,0) = 1;
    B(0,1) = 2;
    B(0,2) = 2;
    B(1,0) = 4;
    B(1,1) = 4;
    B(1,2) = 2;
    B(2,0) = 4;
    B(2,1) = 6;
    B(2,2) = 4;
    PartialPivLU<Matrix3f> lu3(B);
    auto L3 = lu3.matrixL();
    printf("matrix L3\n");
    printf("%f %f %f\n", L3(0,0), L3(0,1), L3(0,2));
    printf("%f %f %f\n", L3(1,0), L3(1,1), L3(1,2));
    printf("%f %f %f\n", L3(2,0), L3(2,1), L3(2,2));

    auto U3 = lu3.matrixU();
    printf("matrix U3\n");
    printf("%f %f %f\n", U3(0,0), U3(0,1), U3(0,2));
    printf("%f %f %f\n", U3(1,0), U3(1,1), U3(1,2));
    printf("%f %f %f\n", U3(2,0), U3(2,1), U3(2,2));

    auto result = L3 * U3;
    printf("result\n");
    printf("%f %f %f\n", result(0,0), result(0,1), result(0,2));
    printf("%f %f %f\n", result(1,0), result(1,1), result(1,2));
    printf("%f %f %f\n", result(2,0), result(2,1), result(2,2));

}