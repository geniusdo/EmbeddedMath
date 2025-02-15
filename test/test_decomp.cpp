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
    auto inv = lu3.inverse();
    printf("invB\n");
    printf("%f %f %f\n", inv(0,0), inv(0,1), inv(0,2));
    printf("%f %f %f\n", inv(1,0), inv(1,1), inv(1,2));
    printf("%f %f %f\n", inv(2,0), inv(2,1), inv(2,2));

    Matrix4f C = Matrix4f::Zero();
    C(0,0) = 1;
    C(0,1) = 2;
    C(0,2) = 3;
    C(0,3) = 4;
    C(1,0) = 2;
    C(1,1) = 3;
    C(1,2) = 2;
    C(1,3) = 5;
    C(2,0) = 1;
    C(2,1) = 2;
    C(2,2) = 3;
    C(2,3) = 2;
    C(3,0) = 1;
    C(3,1) = -1;
    C(3,2) = 0;
    C(3,3) = 2;
    PartialPivLU<Matrix4f> lu4(C);
    auto invC = lu4.inverse();
    printf("invC\n");
    printf("%f %f %f %f\n", invC(0,0), invC(0,1), invC(0,2), invC(0,3));   
    printf("%f %f %f %f\n", invC(1,0), invC(1,1), invC(1,2), invC(1,3));
    printf("%f %f %f %f\n", invC(2,0), invC(2,1), invC(2,2), invC(2,3));
    printf("%f %f %f %f\n", invC(3,0), invC(3,1), invC(3,2), invC(3,3));
}