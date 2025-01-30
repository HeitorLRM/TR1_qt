#ifndef CAMADAFISICA_H
#define CAMADAFISICA_H

#include <cmath>
#include <complex>
#include <vector>

namespace MODULATOR {

static const float amplitude = 1;

float NRZ_polar(bool bit);

float manchester(bool bit, float bit_progress);

float bipolar(bool bit, bool should_update_prev_one = false);

float amp_shift_key(bool bit, float bit_progress);

float freq_shift_key(bool bit, float bit_progress);

float _8_QAM(bool bit, float bit_progress, bool should_update = false);

} // end Modulator



namespace DEMODULATOR {

bool NRZ_polar(const std::vector<float>& signal);

bool manchester(const std::vector<float>& signal);

bool bipolar(const std::vector<float>& signal);

} // end Demodulator

#endif //CAMADAFISICA_H
