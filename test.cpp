#include <cmath>
#include <iostream>
#include <vector>
#include <functional>
#include <algorithm>

#include "rmath.h"
#include "utils/stopwatch.h"
#include "utils/bits.h"

using ld = long double;

std::vector<float> linspace(float s, float e, int n)
{
    std::vector<float> x;
    x.resize(n);

    // [0, n-1] -> [s, e]
    const float scale = (e - s) / (n - 1);
    const float bias = s;

    for (int i = 0; i < n; i++)
    {
        x[i] = i * scale + bias;
    }

    return x;
}

void check(const std::vector<float> &x, const std::function<float(float)> f, const std::function<ld(ld)> tf)
{
    ld rel_error = 0.0;
    ld max_rel_error = 0.0;
    ld abs_error = 0.0;
    ld max_abs_error = 0.0;

    for (int i = 0; i < x.size(); i++)
    {
        const ld err = std::abs(ld(f(x[i])) - tf(x[i]));
        max_abs_error = std::max(err, max_abs_error);
        abs_error += err;

        const ld rerr = err / std::abs(tf(x[i]));
        max_rel_error = std::max(rerr, max_rel_error);
        rel_error += rerr;
    }

    abs_error /= x.size();
    rel_error /= x.size();

    std::cout << "Absolute error: " << abs_error << " (" << max_abs_error << ")" << std::endl;
    std::cout << "Relative error: " << rel_error << " (" << max_rel_error << ")" << std::endl;
}

std::pair<float, float> range(const std::vector<float> &x, const std::function<float(float)> f)
{
    float mx = -1e30;
    float mn = 1e30;

    for (int i = 0; i < x.size(); i++)
    {
        float y = f(x[i]);
        mx = std::max(mx, y);
        mn = std::min(mn, y);
    }

    return std::make_pair(mn, mx);
}

int main()
{
    constexpr int size = 1024 * 1024;

    std::cout << "Checking implementation of sqrt" << std::endl;
    check(linspace(1e-6, 1e4, size), rosh::sqrt, [](ld t) -> ld { return std::sqrt(t); });
    std::cout << "sqrt(-1) = " << rosh::sqrt(-1) << " (-nan)" << std::endl;
    std::cout << std::endl;

    std::cout << "Checking implementation of abs" << std::endl;
    check(linspace(-5, 5, 1024), rosh::abs, [](ld t) -> ld { return std::abs(t); });
    std::cout << std::endl;

    std::cout << "Checking implementation of floor" << std::endl;
    std::cout << "floor(-0.1) = " << rosh::floor(-0.1) << " (-1)" << std::endl;
    std::cout << "floor(-0.0) = " << rosh::floor(-0.0) << " (0)" << std::endl;
    std::cout << "floor(0.1) = " << rosh::floor(0.1) << " (0)" << std::endl;
    std::cout << std::endl;

    std::cout << "Checking implementation of exp" << std::endl;
    check(linspace(-10, 10, size), rosh::exp, [](ld t) -> ld { return std::exp(t); });
    std::cout << "exp(-1e2) = " << rosh::exp(-1e2) << " (0.0)" << std::endl;
    std::cout << "exp(1e2) = " << rosh::exp(1e2) << " (inf)" << std::endl;
    std::cout << std::endl;

    std::cout << "Checking implementation of log" << std::endl;
    check(linspace(1e-6, 10, size), rosh::log, [](ld t) -> ld { return std::log(t); });
    std::cout << "log(-1) = " << rosh::log(-1) << " (nan)" << std::endl;
    std::cout << "log(0) = " << rosh::log(0) << " (-inf)" << std::endl;
    std::cout << "log(inf) = " << rosh::log(rosh::inf) << " (inf)" << std::endl;
    std::cout << std::endl;

    std::cout << "Checking implementation of sin" << std::endl;
    check(linspace(-10, 10, size), rosh::sin, [](ld t) -> ld { return std::sin(t); });
    std::cout << "sin(0) = " << rosh::sin(0) << " (0.0)" << std::endl;
    std::cout << std::endl;

    std::cout << "Checking implementation of cos" << std::endl;
    check(linspace(-10, 10, size), rosh::cos, [](ld t) -> ld { return std::cos(t); });
    std::cout << "cos(0) = " << rosh::cos(0) << " (1.0)" << std::endl;
    std::cout << std::endl;

    std::cout << "Checking implementation of erf" << std::endl;
    check(linspace(-10, 10, size), rosh::erf<false>, [](ld t) -> ld { return std::erf(t); });
    std::cout << "erf(0) = " << rosh::erf(0) << " (0.0)" << std::endl;
    std::cout << "erf(10) = " << rosh::erf(10) << " (1.0)" << std::endl;
    std::cout << "erf(-10) = " << rosh::erf(-10) << " (-1.0)" << std::endl;
    std::cout << std::endl;
}