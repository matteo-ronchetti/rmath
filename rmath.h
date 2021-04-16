#ifndef RMATH_SIMPLE_H
#define RMATH_SIMPLE_H

#include <stdint.h>
#include <limits>

#define EXPECT_FALSE(x) __builtin_expect(x, false)
#define EXPECT_TRUE(x) __builtin_expect(x, true)

#pragma GCC push_options
#pragma GCC optimize ("03", "no-fast-math")

namespace rosh
{
    namespace internals
    {
        float exp_poly(float x)
        {
            float u = 1.8775767e-3f;
            u = u * x + 8.9893401e-3f;
            u = u * x + 5.5826318e-2f;
            u = u * x + 2.4015362e-1f;
            u = u * x + 6.9315307e-1f;
            return u * x + 9.9999993e-1f;
        }

        inline float sin_poly(float x)
        {
            const float xx = x * x;
            float u = -2.0366233e-8f;
            u = u * xx + 2.6998228e-6f;
            u = u * xx + -1.980874e-4f;
            u = u * xx + 8.3324076e-3f;
            u = u * xx + -1.6666553e-1f;
            u = u * xx + 9.999996e-1f;
            return u * x;
        }

        inline float cos_poly(float x)
        {
            const float xx = x * x;
            float u = -2.1978884e-7f;
            u = u * xx + 2.4204402e-5f;
            u = u * xx + -1.3858916e-3f;
            u = u * xx + 4.1659822e-2f;
            u = u * xx + -4.9999427e-1f;
            return u * xx + 9.9999922e-1f;
        }

        inline float erf_poly(float x)
        {
            float u = 0.0000430638f;
            u = u * x + 0.0002765672f;
            u = u * x + 0.0001520143f;
            u = u * x + 0.0092705272f;
            u = u * x + 0.0422820123f;
            u = u * x + 0.0705230784f;
            return u * x + 1.0f;
        }

        inline uint32_t as_uint(float f)
        {
            union
            {
                float f;
                uint32_t i;
            } u = {f};
            return u.i;
        }

        inline float as_float(uint32_t i)
        {
            union
            {
                uint32_t i;
                float f;
            } u = {i};
            return u.f;
        }

        inline uint64_t as_uint64(double f)
        {
            union
            {
                double f;
                uint64_t i;
            } u = {f};
            return u.i;
        }

        inline double as_double(uint64_t i)
        {
            union
            {
                uint64_t i;
                double f;
            } u = {i};
            return u.f;
        }

    }

    float sqrt(float x)
    {
        __asm__("sqrtss %1, %0"
                : "=x"(x)
                : "x"(x));
        return x;
    }

    float hypot(float x, float y)
    {
        // not safe, may overflow
        return sqrt(x * x + y * y);
    }

    float sq(float x)
    {
        return x * x;
    }

    float abs(float x)
    {
        const uint32_t t = reinterpret_cast<const uint32_t &>(x) & 0x7fffffff;
        return reinterpret_cast<const float &>(t);
    }

    // computes abs(x)*sign(y)
    float copy_sign(float x, float y)
    {
        const uint32_t sign = reinterpret_cast<const uint32_t &>(y) & 0x80000000;
        const uint32_t t = reinterpret_cast<const uint32_t &>(x) | sign;
        return reinterpret_cast<const float &>(t);
    }

    float floor(float x)
    {
        return int(x) - (x < 0);
    }
    using T = double;

    constexpr T threshold = 88.7228240;
    constexpr int N = 32;
    constexpr T inv_ln2_n = 0x1.71547652b82fep+0 * 32;
    constexpr T shift = 0x1.8p+52;
    constexpr uint64_t exp_table[32] = {
        0x3ff0000000000000,
        0x3fefd9b0d3158574,
        0x3fefb5586cf9890f,
        0x3fef9301d0125b51,
        0x3fef72b83c7d517b,
        0x3fef54873168b9aa,
        0x3fef387a6e756238,
        0x3fef1e9df51fdee1,
        0x3fef06fe0a31b715,
        0x3feef1a7373aa9cb,
        0x3feedea64c123422,
        0x3feece086061892d,
        0x3feebfdad5362a27,
        0x3feeb42b569d4f82,
        0x3feeab07dd485429,
        0x3feea47eb03a5585,
        0x3feea09e667f3bcd,
        0x3fee9f75e8ec5f74,
        0x3feea11473eb0187,
        0x3feea589994cce13,
        0x3feeace5422aa0db,
        0x3feeb737b0cdc5e5,
        0x3feec49182a3f090,
        0x3feed503b23e255d,
        0x3feee89f995ad3ad,
        0x3feeff76f2fb5e47,
        0x3fef199bdd85529c,
        0x3fef3720dcef9069,
        0x3fef5818dcfba487,
        0x3fef7c97337b9b5f,
        0x3fefa4afa2a490da,
        0x3fefd0765b6e4540,
    };
    // constexpr T exp_poly[3] = {0x1.c6af84b912394p-5 / N / N / N, 0x1.ebfce50fac4f3p-3 / N / N, 0x1.62e42ff0c52d6p-1 / N};
    constexpr T exp_poly[4] = {
        5.5504596955959107902e-2l / N / N / N,
        2.4022885514364502397e-1l / N / N,
        6.9314718052020802757e-1l / N,
        9.9999999992833901766e-1l};


    float exp(float x)
    {
        if (EXPECT_FALSE(x < -threshold))
            return 0.0f;

        if (EXPECT_FALSE(x > threshold))
            return std::numeric_limits<float>::infinity();

        const T xd = T(x);

        T z = xd * inv_ln2_n;
        double kd = z + shift;
        uint64_t ki = internals::as_uint64(kd);
        kd -= shift;

        double r = z - kd;
        /* exp(x) = 2^(k/N) * 2^(r/N) ~= s * (C0*r^3 + C1*r^2 + C2*r + 1) */
        uint64_t t = exp_table[ki % N];
        t += ki << (52 - 5);
        double s = internals::as_double(t);
        z = exp_poly[0] * r + exp_poly[1];
        double r2 = r * r;
        double y = exp_poly[2] * r + exp_poly[3];
        y = z * r2 + y;
        y = y * s;
        
        // z = ((exp_poly[0] * r + exp_poly[1]) * r + exp_poly[2]) * r + exp_poly[3];
        // double y = z * s;

        return (float)y;
    }

    float sin(float x)
    {
        constexpr float twopi = 6.283185307179586f;
        constexpr float invtwopi = 0.15915494309189535f;

        // x is now in range [-pi, pi]
        x -= rosh::floor(x * invtwopi + 0.5f) * twopi;

        return internals::sin_poly(x);
    }

    float cos(float x)
    {
        constexpr float twopi = 6.283185307179586f;
        constexpr float invtwopi = 0.15915494309189535f;

        // x is now in range [-pi, pi]
        x -= rosh::floor(x * invtwopi + 0.5f) * twopi;

        return internals::cos_poly(x);
    }

    // specify sorted = true when you are processing "long" sequences of positive/negative values.
    // long sequences with same sign ===> few branch mispredictions ===> "if" is more perfomant
    template <bool sorted = false>
    float erf(float x)
    {
        if (sorted)
        {
            if (x > 0)
            {
                float y = internals::erf_poly(x);
                y = y * y;
                y = y * y;
                y = y * y;
                return 1.0f - 1.0f / (y * y);
            }
            else
            {
                float y = internals::erf_poly(-x);
                y = y * y;
                y = y * y;
                y = y * y;
                return 1.0f / (y * y) - 1.0f;
            }
        }
        else
        {
            float y = internals::erf_poly(rosh::abs(x));
            y = y * y;
            y = y * y;
            y = y * y;
            y = 1.0f - 1.0f / (y * y);

            return copy_sign(y, x);
        }
    }
}

#pragma GCC pop_options
#endif