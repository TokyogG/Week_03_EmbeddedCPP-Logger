#include <iostream>
#include <vector>
#include <chrono>
#include <cstdint>

#include "conv_naive_s8.h"

extern "C" {
#include "arm_nnfunctions.h"
}

static uint64_t now_ns() {
    using namespace std::chrono;
    return duration_cast<nanoseconds>(high_resolution_clock::now().time_since_epoch()).count();
}

int main() {
    const int H = 32, W = 32;
    const int Cin = 8;
    const int Cout = 8;

    const int kernel_h = 3, kernel_w = 3;
    const int pad = 1;
    const int stride = 1;
    const int dilation = 1;

    // HWC input/output
    std::vector<int8_t> input(H * W * Cin);
    std::vector<int8_t> weights(Cout * kernel_h * kernel_w * Cin);
    std::vector<int32_t> bias(Cout, 0);

    std::vector<int8_t> out_naive(H * W * Cout);
    std::vector<int8_t> out_cmsis(H * W * Cout);

    for (size_t i = 0; i < input.size(); ++i)   input[i] = (int8_t)((i % 17) - 8);
    for (size_t i = 0; i < weights.size(); ++i) weights[i] = (int8_t)((i % 7) - 3);

    const int iters = 200;

    // ---- Naive reference ----
    uint64_t t0 = now_ns();
    for (int i = 0; i < iters; ++i) {
        conv3x3_naive_s8_hwc(input.data(), H, W, Cin, weights.data(), Cout, bias.data(), out_naive.data());
    }
    uint64_t t1 = now_ns();
    double naive_ns = (double)(t1 - t0) / iters;

    // ---- CMSIS-NN wrapper s8 ----
    cmsis_nn_dims input_dims  {1, H, W, Cin};
    cmsis_nn_dims filter_dims {Cout, kernel_h, kernel_w, Cin};
    cmsis_nn_dims bias_dims   {1, 1, 1, Cout};
    cmsis_nn_dims output_dims {1, H, W, Cout};

    cmsis_nn_conv_params conv_params;
    conv_params.padding.h = pad;    conv_params.padding.w = pad;
    conv_params.stride.h  = stride; conv_params.stride.w  = stride;
    conv_params.dilation.h = dilation; conv_params.dilation.w = dilation;

    conv_params.input_offset = 0;
    conv_params.output_offset = 0;
    conv_params.activation.min = -128;
    conv_params.activation.max = 127;

    // IMPORTANT:
    // CMSIS-NN uses per-channel requantization. A multiplier of 1 will effectively scale down to ~0.
    // 0x7fffffff is ~1.0 in Q31.
    std::vector<int32_t> mult(Cout, 0x7fffffff);
    std::vector<int32_t> shift(Cout, 0);

    cmsis_nn_per_channel_quant_params quant_params;
    quant_params.multiplier = mult.data();
    quant_params.shift = shift.data();

    cmsis_nn_context ctx;
    ctx.size = arm_convolve_wrapper_s8_get_buffer_size(&conv_params, &input_dims, &filter_dims, &output_dims);
    std::vector<int8_t> tmp(ctx.size);
    ctx.buf = tmp.data();

    t0 = now_ns();
    for (int i = 0; i < iters; ++i) {
        arm_cmsis_nn_status st = arm_convolve_wrapper_s8(
            &ctx,
            &conv_params,
            &quant_params,
            &input_dims,  input.data(),
            &filter_dims, weights.data(),
            &bias_dims,   bias.data(),
            &output_dims, out_cmsis.data()
        );
        if (st != ARM_CMSIS_NN_SUCCESS) {
            std::cerr << "CMSIS-NN failed with status " << (int)st << "\n";
            return 1;
        }
    }
    t1 = now_ns();
    double cmsis_ns = (double)(t1 - t0) / iters;

    // Compare outputs (may still differ due to requant rounding vs naive clamp)
    int mismatches = 0;
    for (size_t i = 0; i < out_naive.size(); ++i) {
        if (out_naive[i] != out_cmsis[i]) {
            if (++mismatches <= 8) {
                std::cerr << "Mismatch at " << i
                          << " naive=" << (int)out_naive[i]
                          << " cmsis=" << (int)out_cmsis[i] << "\n";
            }
        }
    }

    std::cout << "Naive conv3x3 s8 (HWC): " << naive_ns << " ns/iter\n";
    std::cout << "CMSIS wrapper conv s8:  " << cmsis_ns << " ns/iter\n";
    std::cout << "Speedup: " << (naive_ns / cmsis_ns) << "x\n";
    std::cout << "Mismatches: " << mismatches << " / " << out_naive.size() << "\n";
    return 0;
}
