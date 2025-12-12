#include "conv_naive_s8.h"

static inline int32_t clamp_i32(int32_t x, int32_t lo, int32_t hi) {
    return (x < lo) ? lo : (x > hi) ? hi : x;
}

void conv3x3_naive_s8_hwc(
    const int8_t* input,
    int H, int W, int Cin,
    const int8_t* weights,
    int Cout,
    const int32_t* bias,
    int8_t* output
) {
    const int pad = 1;
    const int Hout = H;
    const int Wout = W;

    for (int y = 0; y < Hout; ++y) {
        for (int x = 0; x < Wout; ++x) {
            for (int oc = 0; oc < Cout; ++oc) {
                int32_t acc = bias ? bias[oc] : 0;

                for (int ky = 0; ky < 3; ++ky) {
                    for (int kx = 0; kx < 3; ++kx) {
                        int in_y = y + ky - pad;
                        int in_x = x + kx - pad;
                        if (in_y < 0 || in_y >= H || in_x < 0 || in_x >= W) continue;

                        const int in_base = (in_y * W + in_x) * Cin;
                        const int w_base  = (((oc * 3 + ky) * 3 + kx) * Cin);

                        for (int ic = 0; ic < Cin; ++ic) {
                            acc += (int32_t)input[in_base + ic] * (int32_t)weights[w_base + ic];
                        }
                    }
                }

                acc = clamp_i32(acc, -128, 127);
                output[(y * Wout + x) * Cout + oc] = (int8_t)acc;
            }
        }
    }
}
