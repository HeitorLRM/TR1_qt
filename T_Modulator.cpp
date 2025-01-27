#include "T_Modulator.hpp"
#include "Media.hpp"
#include "Sync.hpp"
#include "T_WorkerThread.hpp"
#include "T_Settings.hpp"

#include <cmath>
#include <complex>
#include <iostream>
#include <memory>

using Transmitter::Modulator;

void Modulator::update_byte(int) {
	if(data_stream.rdbuf()->in_avail())
		active_byte = char(data_stream.get());
	else
	 	active_byte = 0x0;
}

void Modulator::update_bit(int i) {
	sending_bit = active_byte & (1 << (7-i));
	if (sending_bit) bipolar_prev_one = -bipolar_prev_one;

    float r = (float)(rand()) / (float)(RAND_MAX);
    if (r < Sync::GetTSettings().error_chance)
        sending_bit = !sending_bit;

    T_Worker::Instance()->emit_bit(sending_bit);
}

void Modulator::update_sub(int) {
	float signal = calc_energy(sending_bit);
	Medium::Instance(Medium::WRITE)->transmit(signal);
    T_Worker::Instance()->emit_energy(signal);
}

Modulator* Modulator::Instance() {
	static std::unique_ptr<Modulator> instance(new Modulator);
	return instance.get();
}

Modulator::Modulator() {
}

std::ostream& Modulator::out() {
	return data_stream;
}

float Modulator::calc_energy(bool bit) {
    switch (Sync::GetTSettings().modulation) {
    case T_Settings::MODS::NRZ_POLAR: return NRZ_polar(bit);
    case T_Settings::MODS::MANCHESTER: return manchester(bit);
    case T_Settings::MODS::BIPOLAR: return bipolar(bit);
    case T_Settings::MODS::_ASK: return amp_shift_key(bit);
    case T_Settings::MODS::_FSK: return freq_shift_key(bit);
    case T_Settings::MODS::_4_QAM: return eight_quadrature(bit);
	}
}

float Modulator::NRZ_polar(bool bit) {
	return bit ? amp : -amp;
}

float Modulator::manchester(bool bit) {
	auto progr = Sync::bit_progress();
	return (progr > 0.5) ? NRZ_polar(bit) : -NRZ_polar(bit);
}

float Modulator::bipolar(bool bit) {
	return bit ? -bipolar_prev_one*amp : 0;
}

float Modulator::amp_shift_key(bool bit) {
	float t = 2*M_PI*Sync::bit_progress();
	float carrier = sin(t)*amp;
	return bit * carrier;
}

float Modulator::freq_shift_key(bool bit) {
	float t = 2*M_PI*Sync::bit_progress();
	if (bit) t *= 2;
	float carrier = sin(t)*amp;
	return carrier;
}

float Modulator::eight_quadrature(bool) {
    int quarter = 4.0*(Sync::current_bit() - Sync::current_byte()) / Sync::get_byte_duration();
    char data = active_byte;
    data = data >> (3-quarter);
    data &= 0x3;

    std::complex<float> c;
    switch (data) {
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
    float phase = std::arg(c) + 2*M_PI*Sync::bit_progress();
    float ampl = std::abs(c);
    std::cout << int(data) << ' ' << std::arg(c) << ' ' << ampl << std::endl;
    return ampl * sin(phase);
}
