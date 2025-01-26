#pragma once

#include <chrono>

namespace Sync {

extern unsigned resolution;

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