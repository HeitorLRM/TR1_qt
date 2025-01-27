#pragma once

#include <chrono>
#include "T_Settings.hpp"
#include "R_Settings.hpp"

namespace Sync {

void SetTSettings(T_Settings);
void SetRSettings(R_Settings);
T_Settings GetTSettings();
R_Settings GetRSettings();

void runT(unsigned long = 1);
void TupdateB(int);
void Tupdateb(int);
void Tupdates(int);
void runR(unsigned long = 1);

void set_byte_freq(float);
void set_bit_freq(float);
std::chrono::microseconds get_bit_duration();
std::chrono::microseconds get_byte_duration();

std::chrono::steady_clock::time_point current_byte();
std::chrono::steady_clock::time_point next_byte();
std::chrono::steady_clock::time_point current_bit();
std::chrono::steady_clock::time_point next_bit();

float bit_progress();

}
