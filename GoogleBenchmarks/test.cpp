#include "benchmark/benchmark.h"


void BM_empty(benchmark::State& state) {
  for (auto _ : state) {
    benchmark::DoNotOptimize(state.iterations());
  }
}

BENCHMARK(BM_empty);
BENCHMARK(BM_empty)->ThreadPerCpu();

// Ensure that StateIterator provides all the necessary typedefs required to
// instantiate std::iterator_traits.
static_assert(std::is_same<
  typename std::iterator_traits<benchmark::State::StateIterator>::value_type,
  typename benchmark::State::StateIterator::value_type>::value, "");

BENCHMARK_MAIN();
