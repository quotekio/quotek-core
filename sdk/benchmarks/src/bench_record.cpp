#include <benchmark/benchmark.h>
#include <quotek/record.hpp>


/** Benchmarks the creation of records type object */
static void BM_records_creation(benchmark::State& state) {
  while (state.KeepRunning())
    quotek::data::records r1;
}

BENCHMARK(BM_records_creation);

/** Benchmarks the allocation and storage of 10000 new records to data. */
static void BM_records_10k_assign_via_record(benchmark::State& state) {
  
  quotek::data::records r1;
  int i= 0;

  while (state.KeepRunning()) {

    for (i=0; i< 10000; i++) {
      quotek::data::record rc1(i,i/1.0,0);
      r1.append(rc1);
    }

  }
}

BENCHMARK(BM_records_10k_assign_via_record);

/** Benchmarks the allocation and storage of 10000 new records to data, value only */
static void BM_records_10k_assign_via_value(benchmark::State& state) {

  quotek::data::records r1;
  int i= 0;

  while (state.KeepRunning()) {
    for (i=0; i< 10000; i++) {
      double v = i;
      r1.append(i);
    }

  }
}

BENCHMARK(BM_records_10k_assign_via_value);

/** Benchmarks the extraction of 500 records at the middle of dataset */
static void BM_records_10k_extract_500(benchmark::State& state) {

  quotek::data::records r1;
  int i= 0;
  for (i=0; i< 10000; i++) {
      double v = i;
      r1.append(i);
  }

  while (state.KeepRunning()) {
    quotek::data::records r2 = r1.extract(4750,500);
  }

}

BENCHMARK(BM_records_10k_extract_500);




BENCHMARK_MAIN();
