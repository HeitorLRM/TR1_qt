
#include "R_Demodulator.hpp"
#include "R_WorkerThread.hpp"
#include "R_Settings.hpp"
#include "Media.hpp"
#include "Sync.hpp"

#include <memory>
#include <thread>

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
    switch (Sync::GetRSettings().modulation) {
    case R_Settings::MODS::NRZ_POLAR: return NRZ_polar();
    case R_Settings::MODS::MANCHESTER: return manchester();
    case R_Settings::MODS::BIPOLAR: return bipolar();
	}
	return false;
}

bool Demodulator::NRZ_polar() {
    unsigned midpoint = Sync::GetRSettings().resolution/2;
    float energy;
    for (unsigned i = 0; i < Sync::GetRSettings().resolution; i++) {
        float listen = Medium::Instance(Medium::READ)->listen();
        if (i == midpoint) energy = listen;
        std::this_thread::sleep_until(Sync::current_bit() + Sync::get_bit_duration()/Sync::GetRSettings().resolution);
        R_Worker::Instance()->emit_energy(listen);
    }

	return energy > 0;
}

bool Demodulator::manchester() {
    auto quarterpoint = Sync::current_bit() + Sync::get_bit_duration()*0.75;
	std::this_thread::sleep_until(quarterpoint);
	float energy = Medium::Instance(Medium::READ)->listen();
    return energy > 0;

}

bool Demodulator::bipolar() {
	static const float threshold = 0.5;
	auto midpoint = Sync::current_bit() + Sync::get_bit_duration()/2;
	std::this_thread::sleep_until(midpoint);
	float energy = Medium::Instance(Medium::READ)->listen();
	return (energy > threshold*amp || energy < -threshold*amp);
}
