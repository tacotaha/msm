#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <sys/random.h>

#include "bindings/blst.h"
#include "msm.h"

#define RUNS (1 << 10)

static inline uint64_t cycle_count() {
#ifdef __aarch64__
  uint64_t cc;
  asm volatile ("mrs %0, cntvct_el0":"=r" (cc));
  return cc;
#else
  uint64_t low, high;
  asm volatile ("rdtsc":"=a" (low), "=d"(high));
  return (high << 32) | low;
#endif
}

int main() {
  blst_scalar scalars[N];
  blst_p1 res1, res2, pts[N];
  long long cc_start, cc_stop, c_avg1 = 0, c_avg2 = 0;
  blst_p1_affine pts_affine[N];
  blst_p1 const *G1 = blst_p1_generator();

  for (int i = 0; i < RUNS; ++i) {
    memset(windows, 0, sizeof(windows));
    memset(scratch, 0, sizeof(scratch));

    getrandom(scalars, sizeof(scalars), GRND_RANDOM);

    for (int j = 0; j < N; ++j)
      blst_p1_mult(pts + j, G1, scalars[j].b, 256);

    getrandom(scalars, sizeof(scalars), GRND_RANDOM);

    const blst_p1 *const p1_arg[2] = { pts, NULL };
    blst_p1s_to_affine(pts_affine, p1_arg, N);

    cc_start = cycle_count();
    msm(&res1, pts_affine, scalars, N);
    cc_stop = cycle_count();
    c_avg1 += (cc_stop - cc_start);

    cc_start = cycle_count();
    blst_msm(&res2, pts_affine, scalars, N);
    cc_stop = cycle_count();
    c_avg2 += (cc_stop - cc_start);

    assert(blst_p1_is_equal(&res1, &res2));
  }

  c_avg1 /= RUNS;
  c_avg2 /= RUNS;

  printf("%lld %lld\n", c_avg1, c_avg2);

  return 0;
}
