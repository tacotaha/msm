#ifndef MSM_H
#define MSM_H

#include "bindings/blst.h"

#define N (1 << 10)
#define P1_SCRATCH_SIZE (sizeof(blst_p1) * (N < 8192 ? N : sizeof(blst_p1)))

#define W 8
#define L 256

#define WINDOW_MASK ((1 << W) - 1)
#define N_BUCKETS (1 << W)
#define N_WINDOWS (L / W)

extern limb_t scratch[P1_SCRATCH_SIZE];
extern blst_p1_xyzz windows[N_WINDOWS];

void msm(blst_p1 *, blst_p1_affine *, blst_scalar *, size_t);
void blst_msm(blst_p1 *, blst_p1_affine *, blst_scalar *, size_t);
void lincomb(blst_p1 *, blst_p1 *, const blst_scalar *, size_t);

#endif                          /* MSM_H */
