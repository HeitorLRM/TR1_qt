#include "Sync.hpp"
#include "T_Modulator.hpp"
#include "T_Settings.hpp"
#include <chrono>
#include <thread>

using namespace std::chrono;

static microseconds bit_duration = 200ms;
static steady_clock::time_point begint = steady_clock::now();

static T_Settings tsettings;
void Sync::SetTSettings(T_Settings s) {
    tsettings = s;
    set_bit_freq(s.frequency);
}

static R_Settings rsettings;
void Sync::SetRSettings(R_Settings s) {
    rsettings = s;
    set_bit_freq(s.frequency);
}

T_Settings Sync::GetTSettings() {
    return tsettings;
}

R_Settings Sync::GetRSettings() {
    return rsettings;
}

void Sync::runT(unsigned long b_cycles) {
	for (unsigned long i = 0; i < b_cycles; i++) {
		auto nB = next_byte();

		TupdateB(i);

		for (char j = 0; j < 8; j++) {
			auto nb = next_bit();

			Tupdateb(j);

			auto cb = current_bit();
            auto sub_interval = Sync::get_bit_duration() / GetTSettings().resolution;
            for (unsigned k = 0; k < GetTSettings().resolution; k++) {
				Tupdates(k);
				std::this_thread::sleep_until(cb + (k+1)*sub_interval);
			}

			std::this_thread::sleep_until(nb);
		}	
		std::this_thread::sleep_until(nB);
	}
}

void Sync::TupdateB(int i) {
	Transmitter::Modulator::Instance()->update_byte(i);
}

void Sync::Tupdateb(int i) {
	Transmitter::Modulator::Instance()->update_bit(i);
}

void Sync::Tupdates(int i) {
	Transmitter::Modulator::Instance()->update_sub(i);
}

void Sync::set_byte_freq(float freq) {
	set_bit_freq(8 * freq);
}

void::Sync::set_bit_freq(float freq) {
	bit_duration = microseconds(int(1e+6f/freq));
}

std::chrono::microseconds Sync::get_bit_duration() {
	return bit_duration;
}

std::chrono::microseconds Sync::get_byte_duration() {
	return 8*bit_duration;
}

steady_clock::time_point Sync::current_byte() {
	steady_clock::time_point endt = steady_clock::now();
	auto diff = duration_cast<microseconds>(endt-begint);
	auto frame_count = diff / get_byte_duration();
	return begint + frame_count*get_byte_duration();
}

steady_clock::time_point Sync::next_byte() {
	return current_byte() + get_byte_duration();
}

steady_clock::time_point Sync::current_bit() {
	steady_clock::time_point endt = steady_clock::now();
	auto diff = duration_cast<microseconds>(endt-begint);
	long bit_count = diff / bit_duration;
	return begint + bit_count*bit_duration;
}

steady_clock::time_point Sync::next_bit() {
	return current_bit() + bit_duration;
}

float Sync::bit_progress() {
	microseconds delta = duration_cast<microseconds>(steady_clock::now() - current_bit());
	return float(delta.count()) / float(bit_duration.count());
}


float Sync::byte_progress() {
    microseconds delta = duration_cast<microseconds>(steady_clock::now() - current_byte());
    return float(delta.count()) / float(get_byte_duration().count());
}
