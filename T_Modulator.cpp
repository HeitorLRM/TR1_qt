#include "T_Modulator.hpp"
#include "Media.hpp"
#include "Sync.hpp"
#include "T_WorkerThread.hpp"
#include "T_Settings.hpp"
#include "CamadaFisica.hpp"

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

    float r = (float)(rand()) / (float)(RAND_MAX);
    if (r < Sync::GetTSettings().error_chance)
        sending_bit = !sending_bit;

    T_Worker::Instance()->emit_bit(sending_bit);
    first_sub = true;
}

void Modulator::update_sub(int) {
	float signal = calc_energy(sending_bit);
	Medium::Instance(Medium::WRITE)->transmit(signal);
    T_Worker::Instance()->emit_energy(signal);
    first_sub = false;
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
    auto bit_progress = Sync::bit_progress();
    auto byte_progress = Sync::byte_progress();

    switch (Sync::GetTSettings().modulation) {
    case T_Settings::MODS::NRZ_POLAR: return MODULATOR::NRZ_polar(bit);
    case T_Settings::MODS::MANCHESTER: return MODULATOR::manchester(bit, bit_progress);
    case T_Settings::MODS::BIPOLAR: return MODULATOR::bipolar(bit, first_sub);
    case T_Settings::MODS::_ASK: return MODULATOR::amp_shift_key(bit, bit_progress);
    case T_Settings::MODS::_FSK: return MODULATOR::freq_shift_key(bit, bit_progress);
    case T_Settings::MODS::_8_QAM: return MODULATOR::_8_QAM(bit, bit_progress, first_sub);
    }
    return 0;
}
