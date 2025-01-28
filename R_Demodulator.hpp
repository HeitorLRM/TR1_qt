#pragma once

#include "R_Settings.hpp"
#include <vector>

namespace Receiver {

class Demodulator {
private:
	Demodulator();

public:
	static Demodulator* Instance();

	bool read_bit();
	char read_byte();

private:
	bool calc_bit();
};

}
