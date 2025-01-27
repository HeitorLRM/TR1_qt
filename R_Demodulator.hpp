#pragma once

#include "R_Settings.hpp"

namespace Receiver {

class Demodulator {
private:
	Demodulator();

public:
	static Demodulator* Instance();

	bool read_bit();
	char read_byte();

    R_Settings::MODS modulation = R_Settings::MODS::NRZ_POLAR;

private:
	bool calc_bit();
	bool NRZ_polar();
	bool manchester();
	bool bipolar();

	float amp = 1;
};

}
