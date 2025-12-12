#pragma once
#include <cstdint>

void conv3x3_naive_s8_hwc(
    const int8_t* input,
    int H, int W, int Cin,
    const int8_t* weights,
    int Cout,
    const int32_t* bias,
    int8_t* output
);
