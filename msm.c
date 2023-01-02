#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <omp.h>

#include "bindings/blst.h"
#include "msm.h"

limb_t scratch[P1_SCRATCH_SIZE] = { 0 };
blst_p1_xyzz windows[N_WINDOWS] = { 0 };

void msm(blst_p1 * res, blst_p1_affine * pts, blst_scalar * scalars, size_t n) {
  blst_p1_xyzz sum = { 0 };

#pragma omp parallel for
  for (int i = 0; i < N_WINDOWS; ++i) {
    unsigned char window;
    blst_p1_xyzz buckets[N_BUCKETS] = { 0 }, sum = { 0 };

    for (size_t j = 0; j < n; ++j) {
      window = scalars[j].b[i];
      if (window)
        vartime_blst_p1xyzz_dadd_affine(buckets + window - 1, buckets + window - 1, pts + j, 0);
    }

    memset(&sum, 0, sizeof(sum));
    for (int j = N_BUCKETS - 1; j > -1; --j) {
      vartime_blst_p1xyzz_dadd(&sum, buckets + j, &sum);
      vartime_blst_p1xyzz_dadd(windows + i, &sum, windows + i);
      memset(buckets + j, 0, sizeof(blst_p1_xyzz));
    }
  }

  for (int j = N_WINDOWS - 1; j > -1; --j) {
    for (int k = 0; k < W; ++k)
      vartime_blst_p1xyzz_dadd(&sum, &sum, &sum);
    vartime_blst_p1xyzz_dadd(&sum, windows + j, &sum);
  }

  blst_p1xyzz_to_Jacobian(res, &sum);
}

void blst_msm(blst_p1 * res, blst_p1_affine * pts, blst_scalar * scalars, size_t n) {
  const blst_p1_affine *const pts_arg[2] = { pts, NULL };
  const unsigned char *const scalars_arg[2] = { (unsigned char *) scalars, NULL };
  blst_p1s_mult_pippenger(res, pts_arg, n, scalars_arg, 256, scratch);
}

void lincomb(blst_p1 * res, blst_p1 * pts, const blst_scalar * scalars, size_t n) {
  blst_p1 tmp;
  memset(res, 0, sizeof(blst_p1));
  for (size_t i = 0; i < n; ++i) {
    blst_p1_mult(&tmp, pts + i, scalars[i].b, 256);
    blst_p1_add_or_double(res, res, &tmp);
  }
}
