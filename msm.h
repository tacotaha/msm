#ifndef MSM_H
#define MSM_H

#include "bindings/blst.h"

#define N (1 << 10)
#define P1_SCRATCH_SIZE (sizeof(blst_p1) * (N < 8192 ? N : sizeof(blst_p1)))

#define W 8
#define L 256
#define N_WINDOWS (L / W)
#define N_BUCKETS (1 << (W - 1))
#define WIDTH (1 << (W - 1))

typedef struct signed_scalar_t {
  int8_t b[N_WINDOWS + 1];
} signed_scalar_t;

static inline void sign_encode(signed_scalar_t * out, const blst_scalar * in) {
  uint32_t window, rem = 0;
  for (int i = 0; i < N_WINDOWS; ++i) {
    window = rem + in->b[i];
    rem = (window + WIDTH) >> W;
    out->b[i] = window - (rem << W);
  }
  out->b[N_WINDOWS] = rem;
}

extern limb_t scratch[P1_SCRATCH_SIZE];
extern blst_p1_xyzz windows[N_WINDOWS];

void msm(blst_p1 *, blst_p1_affine *, blst_scalar *, size_t);
void blst_msm(blst_p1 *, blst_p1_affine *, blst_scalar *, size_t);
void lincomb(blst_p1 *, blst_p1 *, const blst_scalar *, size_t);

#endif                          /* MSM_H */
