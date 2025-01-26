#include "T_Modulator.hpp"
#include "Media.hpp"
#include "Sync.hpp"
#include <cmath>
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
}

void Modulator::update_sub(int) {
	float signal = calc_energy(sending_bit);
	Medium::Instance(Medium::WRITE)->transmit(signal);
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
	switch (modulation) {
	case NRZ_POLAR: return NRZ_polar(bit);
	case MANCHESTER: return manchester(bit);
	case BIPOLAR: return bipolar(bit);
	case _ASK: return amp_shift_key(bit);
	case _FSK: return freq_shift_key(bit);
	case _8_QAM: return eight_quadrature(bit);
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
	return 0;
}
