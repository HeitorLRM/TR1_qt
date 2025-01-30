#include "CamadaFisica.hpp"

float MODULATOR::NRZ_polar(bool bit) {
    // +V para bit 1, -V quando 0.
    return bit ? amplitude : -amplitude;
}

float MODULATOR::manchester(bool bit, float bit_progress) {
    // O sinal é igual ao NRZ_Polar para a primeira metade do periodo e o oposto para a segunda metade.
    return (bit_progress > 0.5) ? NRZ_polar(bit) : -NRZ_polar(bit);
}

float MODULATOR::bipolar(bool bit, bool should_update_prev_one) {
    // Bipolar é zero quando o bit é zero.
    if (!bit) return 0;

    // O bit 1 é o oposto da sinal mandado pelo 1 pela ultima vez
    // Guardar memória do sinal do ultimo bit 1 mandado
    static char prev_one = -1;
    if (should_update_prev_one) // Para fins de simulação
        prev_one = -prev_one; // Quando emite, o novo sinal será oposto

    return prev_one*amplitude;
}

float MODULATOR::amp_shift_key(bool bit, float bit_progress) {
    const float freq = 1.0;
    const float mult = 0.0;

    // ASK é um sinal senoidal
    float t = freq * 2.0 * M_PI * bit_progress;
    float carrier = sin(t);
    return carrier * (bit ? amplitude : amplitude*mult); // que a amplitude varia com o bit.
}

float MODULATOR::freq_shift_key(bool bit, float bit_progress) {
    const float freq_1 = 2.0;
    const float freq_0 = 1.0;

    // FSK é um sinal senoidal
    float t = 2.0 * M_PI * bit_progress;
    t *= bit ? freq_1 : freq_0; // cuja frequencia depende do bit.
    float carrier = sin(t);

    return carrier * amplitude;
}

float MODULATOR::_8_QAM(bool bit, float bit_progress, bool should_update) {
    // Código para acumular 3 bits
    static unsigned b_count = 0;
    static unsigned char tribit = 0;
    static unsigned char incoming_tribit = 0;
    if (should_update) {
        incoming_tribit <<= 1;
        if (bit) incoming_tribit++;
        b_count++;
        b_count %= 3;
        if (!b_count) {
            tribit = incoming_tribit;
            incoming_tribit = 0;
        }
    }

    // Queremos obter somente um trio de bits
    // Disso, dividimos o periodo do byte e lidamos com bits de tres a tres.

    // O 8-QAM consegue codificar 3 bits em termos de fase e amplitude
    std::complex<float> c; // numeros complexos em coordenadas polares tem fase e amplitude.
    float l = 0.707106781185; // sqrt(2)/2
    // A seguinte é a tabela que mapeia os 3 bits para numeros complexos.

    switch (tribit) {
    case 0x0: c = { l,-l}; break;
    case 0x1: c = { 1, 0}; break;
    case 0x2: c = { l, l}; break;
    case 0x3: c = { 0, 1}; break;
    case 0x4: c = {-l, l}; break;
    case 0x5: c = {-1, 0}; break;
    case 0x6: c = {-l,-l}; break;
    case 0x7: c = { 0,-1}; break;
    default: break;
    }
    float phase    = std::arg(c);
    float amp_mult = std::abs(c); // Obtemos fase e amplitude

    float tribit_progress = (b_count + bit_progress)/3.0; // O periodo engloba 3 bits
    float t = 2.0 * M_PI * tribit_progress + phase;
    float carrier = sin(t);
    float amp = amplitude * amp_mult;

    return amp * carrier;
}


bool DEMODULATOR::NRZ_polar(const std::vector<float>& signal) {
    // Amostramos um ponto no meio do periodo para não sincronizarmos com o transmissor
    unsigned midpoint = signal.size() / 2;
    float energy = signal[midpoint];
    return energy > 0;
}

bool DEMODULATOR::manchester(const std::vector<float>& signal) {
    // Amostramos dois pontos da onda onde os sinais são opostos
    unsigned point_0 = 0.25 * signal.size();
    unsigned point_1 = 0.75 * signal.size();
    float energy_0 = signal[point_0];
    float energy_1 = signal[point_1];

    // O bit 1 é tal que há um flanco de subida dentro do periodo
    return (energy_0 < 0) && (energy_1 > 0);
}

bool DEMODULATOR::bipolar(const std::vector<float>& signal) {
    // O bipolar interpreta como 1 quando seu sinal supera um limiar positivo ou negativo
    const float threshold = 0.5;

    unsigned midpoint = signal.size() / 2; // Amostramos um ponto médio
    float energy = signal[midpoint];
    return std::abs(energy) > threshold * MODULATOR::amplitude;
}
