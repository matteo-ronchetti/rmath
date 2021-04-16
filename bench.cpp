#include <benchmark/benchmark.h>
#include <cmath>
#include <vector>
#include <algorithm>
#include "rmath.h"

constexpr int size = 1024 * 64;

#define BENCH(name, f, s, e)                  \
    static void name(benchmark::State &state) \
    {                                         \
    auto x = linspace(s, e, size);         \
        auto y = linspace(-10, 10, size);     \
        random_shuffle(x.begin(), x.end());   \
        for (auto _ : state)                  \
            for (int i = 0; i < size; i++)    \
                y[i] = f(x[i]);        \
    } \
    BENCHMARK(name);


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

BENCH(sin_math, std::sin, -10, 10);
BENCH(sin_rmath, rosh::sin, -10, 10);

BENCH(cos_math, std::cos, -10, 10);
BENCH(cos_rmath, rosh::cos, -10, 10);

BENCH(exp_math, std::exp, -10, 10);
BENCH(exp_rmath, rosh::exp, -10, 10);

BENCH(log_math, std::log, 1e-6, 10);
BENCH(log_rmath, rosh::log, 1e-6, 10);

BENCH(erf_math, std::erf, -10, 10);
BENCH(erf_rmath, rosh::erf, -10, 10);

BENCHMARK_MAIN();