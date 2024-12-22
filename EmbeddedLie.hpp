// Copyright 2024 Liu Chuangye @ chuangyeliu0206@gmail.com
// Apache Licence 2.0
#ifndef EMBEDDEDLIE_HPP
#define EMBEDDEDLIE_HPP

#include "EmbeddedMath.hpp"

namespace EmbeddedLie
{
    using namespace EmbeddedMath;

    // returnS skew symmetric matrix
    template <typename Scalar>
    inline Matrix<Scalar, 3, 3> skew(const Matrix<Scalar, 3, 1> &w)
    {
        Matrix<Scalar, 3, 3> w_x;
        w_x(0, 1) = -w(2);
        w_x(0, 2) = w(1);
        w_x(1, 0) = w(2);
        w_x(1, 2) = -w(0);
        w_x(2, 0) = -w(1);
        w_x(2, 1) = w(0);
        return w_x;
    }

    // returns vector portion of skew-symmetric
    template <typename Scalar>
    inline Matrix<Scalar, 3, 1> vee(const Matrix<Scalar, 3, 3> &w_x)
    {
        Matrix<Scalar, 3, 1> w(w_x(2, 1), w_x(0, 2), w_x(1, 0));
        return w;
    }

    // we use 4th-order estimation for SO3 exp
    // NOTE: this function is an estimation. When norm > 1, do not use it;
    template <typename Scalar>
    inline Matrix<Scalar, 3, 3> so3_Exp(const Matrix<Scalar, 3, 1> &w)
    {
        Matrix<Scalar, 3, 3> w_x = skew(w);
        Scalar theta_sq = w.dot(w);
        Scalar A, B;
        Matrix<Scalar, 3, 3> R;
        if (theta_sq < FLOAT_EPSILON * FLOAT_EPSILON)
        {
            A = 1;
            B = 0.5;
            if (theta_sq == Scalar(0))
                R = Matrix<Scalar, 3, 3>::Identity(3, 3);
            else
            {
                R = Matrix<Scalar, 3, 3>::Identity() + A * w_x + B * w_x * w_x;
            }
        }
        else
        {
            Scalar theta_quad = theta_sq * theta_sq;
            A = 1 - theta_sq / 6.0 + theta_quad / 120.0;
            B = 0.5 - theta_sq / 24.0 + theta_quad / 720.0;
            R = Matrix<Scalar, 3, 3>::Identity() + A * w_x + B * w_x * w_x;
        }
        return R;
    }

    // output angle range (-pi, pi]
    template <typename Scalar>
    inline Matrix<Scalar, 3, 1> so3_Log(const Matrix<Scalar, 3, 3> &R)
    {

        Scalar a = 0.5 * (R.trace() - 1);
        Scalar theta = (a >= 1) ? 0 : ((a <= -1) ? M_PI : acos(a));
        Scalar D;
        if (theta < FLOAT_EPSILON)
        {
            D = 0.5;
        }
        else if (abs(sin(theta)) < FLOAT_EPSILON)
        {
            Matrix<Scalar, 3, 1> vec;

            vec.x() = sqrt((R(0, 0) + 1) * M_PI_2 * M_PI);

            vec.y() = sqrt((R(1, 1) + 1) * M_PI_2 * M_PI);

            vec.z() = sqrt((R(2, 2) + 1) * M_PI_2 * M_PI);
            return vec;
        }
        else
        {
            D = theta / (2 * sin(theta));
        }

        Matrix<Scalar, 3, 3> w_x = D * (R - R.transpose());

        if (R != Matrix<Scalar, 3, 3>::Identity(3, 3))
        {
            Matrix<Scalar, 3, 1> vec(w_x(2, 1), w_x(0, 2), w_x(1, 0));
            return vec;
        }
        else
        {
            return Matrix<Scalar, 3, 1>::Zero();
        }
    }

    // exp map of hamilton quaternion
    // TODO: add to unit test
    template <typename Scalar>
    inline Quaternion<Scalar> quat_Exp(const Matrix<Scalar, 3, 1> &w)
    {
        Scalar squared_norm = w.dot(w);
        Quaternion<Scalar> q;
        q.setIdentity();
        if (squared_norm < FLOAT_EPSILON * FLOAT_EPSILON)
        {
            if (squared_norm = Scalar(0))
                return q;
            else
            {
                q.w() = 1;
                q.vec() = 0.5 * w;
                q.normalize();
                return q;
            }
        }
        q.w() = 1 - 0.125 * squared_norm + squared_norm * squared_norm / 384;
        q.vec() = (0.5 - squared_norm / (48.0) + squared_norm * squared_norm / (3840.0)) * w;
        q.normalize();
        return q;
    }

    // converts a rotation matrix to JPL quaternion
    // taken from openVins@https://github.com/rpng/open_vins
    // if we want a Hamilton Convention of quaternion simply use the constructor of  Quaternion
    template <typename Scalar>
    inline Matrix<Scalar, 4, 1> rot_2_quat(const Matrix<Scalar, 3, 3> &rot)
    {
        Matrix<Scalar, 4, 1> q;
        Scalar T = rot.trace();
        if ((rot(0, 0) >= T) && (rot(0, 0) >= rot(1, 1)) && (rot(0, 0) >= rot(2, 2)))
        {
            // cout << "case 1- " << endl;
            q(0) = sqrt((1 + (2 * rot(0, 0)) - T) / 4);
            q(1) = (1 / (4 * q(0))) * (rot(0, 1) + rot(1, 0));
            q(2) = (1 / (4 * q(0))) * (rot(0, 2) + rot(2, 0));
            q(3) = (1 / (4 * q(0))) * (rot(1, 2) - rot(2, 1));
        }
        else if ((rot(1, 1) >= T) && (rot(1, 1) >= rot(0, 0)) && (rot(1, 1) >= rot(2, 2)))
        {
            // cout << "case 2- " << endl;
            q(1) = sqrt((1 + (2 * rot(1, 1)) - T) / 4);
            q(0) = (1 / (4 * q(1))) * (rot(0, 1) + rot(1, 0));
            q(2) = (1 / (4 * q(1))) * (rot(1, 2) + rot(2, 1));
            q(3) = (1 / (4 * q(1))) * (rot(2, 0) - rot(0, 2));
        }
        else if ((rot(2, 2) >= T) && (rot(2, 2) >= rot(0, 0)) && (rot(2, 2) >= rot(1, 1)))
        {
            // cout << "case 3- " << endl;
            q(2) = sqrt((1 + (2 * rot(2, 2)) - T) / 4);
            q(0) = (1 / (4 * q(2))) * (rot(0, 2) + rot(2, 0));
            q(1) = (1 / (4 * q(2))) * (rot(1, 2) + rot(2, 1));
            q(3) = (1 / (4 * q(2))) * (rot(0, 1) - rot(1, 0));
        }
        else
        {
            // cout << "case 4- " << endl;
            q(3) = sqrt((1 + T) / 4);
            q(0) = (1 / (4 * q(3))) * (rot(1, 2) - rot(2, 1));
            q(1) = (1 / (4 * q(3))) * (rot(2, 0) - rot(0, 2));
            q(2) = (1 / (4 * q(3))) * (rot(0, 1) - rot(1, 0));
        }
        if (q(3) < 0)
        {
            q = -q;
        }
        // normalize and return
        q = q / (q.norm());
        return q;
    }

    template <typename Scalar>
    inline Matrix<Scalar, 3, 3> Jl_so3(const Matrix<Scalar, 3, 1> &w)
    {
        Scalar theta_sq = w.dot(w);
        Scalar theta = sqrt(theta_sq);

        Matrix<Scalar, 3, 3> Omega = skew(w);

        if (theta_sq < FLOAT_EPSILON * FLOAT_EPSILON)
        {
            return Matrix<Scalar, 3, 3>::Identity() + Scalar(0.5) * Omega;
        }
        else
        {
            if (theta < FLOAT_EPSILON)
            {
                return Matrix<Scalar, 3, 3>::Identity() + Scalar(0.5) * Omega;
            }
            Matrix<Scalar, 3, 3> J = Matrix<Scalar, 3, 3>::Identity() + (Scalar(1.0) - cos(theta)) / theta_sq * Omega + (theta - sin(theta)) / (theta_sq * theta) * Omega * Omega;
            return J;
        }
    }

    template <typename Scalar>
    inline Matrix<Scalar, 3, 3> Jr_so3(const Matrix<Scalar, 3, 1> &w)
    {
        return Jl_so3(Matrix<Scalar, 3, 1>(-w));
    }

} // namespace EmbeddedLie
#endif // EMBEDDEDLIE_HPP