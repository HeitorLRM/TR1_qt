
#include "R_Demodulator.hpp"
#include "R_WorkerThread.hpp"
#include "R_Settings.hpp"
#include "Media.hpp"
#include "Sync.hpp"
#include "CamadaFisica.hpp"

#include <memory>
#include <thread>
#include <iostream>

using Receiver::Demodulator;

Demodulator* Demodulator::Instance() {
	static std::unique_ptr<Demodulator> instance(new Demodulator);
	return instance.get();
}

Demodulator::Demodulator() {
}

char Demodulator::read_byte() {
	auto next_byte = Sync::next_byte();

	char c = 0x0;
	for (int i = 0; i++ < 8;) {
		c <<= 1;
		c |= read_bit() ? 0x1: 0x0;
	}

	std::this_thread::sleep_until(next_byte);
	return c;
}

bool Demodulator::read_bit() {
	auto sync_bit = Sync::next_bit();

	bool b = calc_bit();
    R_Worker::Instance()->emit_bit(b);

	std::this_thread::sleep_until(sync_bit);
	return b;
}

bool Demodulator::calc_bit() {
    unsigned resolution = Sync::GetRSettings().resolution;

    std::vector<float> signal;
    signal.reserve(resolution);

    auto cb = Sync::current_bit();
    auto sub_duration = Sync::get_bit_duration() / resolution;
    for (unsigned i = 0; i < resolution; i++) {
        float listened = Medium::Instance(Medium::READ)->listen();
        signal.push_back(listened);
        R_Worker::Instance()->emit_energy(listened);
        std::this_thread::sleep_until(cb + i*sub_duration);
    }

    switch (Sync::GetRSettings().modulation) {
    case R_Settings::MODS::NRZ_POLAR: return DEMODULATOR::NRZ_polar(signal);
    case R_Settings::MODS::MANCHESTER: return DEMODULATOR::manchester(signal);
    case R_Settings::MODS::BIPOLAR: return DEMODULATOR::bipolar(signal);
	}
	return false;
}
