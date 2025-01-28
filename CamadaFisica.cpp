#include "CamadaFisica.hpp"

float MODULATOR::NRZ_polar(bool bit) {
    return bit ? amplitude : -amplitude;
}

float MODULATOR::manchester(bool bit, float bit_progress) {
    return (bit_progress > 0.5) ? NRZ_polar(bit) : -NRZ_polar(bit);
}

float MODULATOR::bipolar(bool bit, bool should_update_prev_one) {
    if (!bit) return 0;

    static char prev_one = -1;
    if (should_update_prev_one) prev_one = -prev_one;

    return bit ? prev_one*amplitude : 0;
}

float MODULATOR::amp_shift_key(bool bit, float bit_progress) {
    const float freq = 1.0;

    float t = freq * 2.0 * M_PI * bit_progress;
    float carrier = sin(t);
    return carrier * (bit ? amplitude : 0);
}

float MODULATOR::freq_shift_key(bool bit, float bit_progress) {
    const float freq_1 = 2.0;
    const float freq_0 = 1.0;

    float t = 2.0 * M_PI * bit_progress;
    t *= bit ? freq_1 : freq_0;
    float carrier = sin(t);

    return carrier * amplitude;
}

float MODULATOR::_4_quadrature(char byte_data, float byte_progress) {
    int quarter = 4.0 * byte_progress;
    byte_data = byte_data >> (3-quarter);
    byte_data &= 0x3; // 0b00000011

    std::complex<float> c;
    switch (byte_data) {
    case 0x0:
        c = {1,1};
        break;
    case 0x1:
        c = {1,-1};
        break;
    case 0x2:
        c = {-1,1};
        break;
    case 0x3:
        c = {-1,-1};
        break;
    }
    float phase    = std::arg(c);
    float amp_mult = std::abs(c);

    float dibit_progress = 4.0 * byte_progress - quarter;
    float t = 2.0 * M_PI * dibit_progress + phase;
    float carrier = sin(t);
    float amp = amplitude * amp_mult;

    return amp * carrier;
}


bool DEMODULATOR::NRZ_polar(const std::vector<float>& signal) {
    unsigned midpoint = signal.size() / 2;
    float energy = signal[midpoint];
    return energy > 0;
}

bool DEMODULATOR::manchester(const std::vector<float>& signal) {
    unsigned point_0 = 0.25 * signal.size();
    unsigned point_1 = 0.75 * signal.size();
    float energy_0 = signal[point_0];
    float energy_1 = signal[point_1];

    return (energy_0 < 0) && (energy_1 > 0);
}

bool DEMODULATOR::bipolar(const std::vector<float>& signal) {
    const float threshold = 0.5;

    unsigned midpoint = signal.size() / 2;
    float energy = signal[midpoint];
    return std::abs(energy) > threshold * MODULATOR::amplitude;
}
