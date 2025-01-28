#pragma once

#include "T_Settings.hpp"

#include <ostream>
#include <sstream>

namespace Transmitter {

class Modulator {
private:
	Modulator();

public:
	void update_byte(int);
	void update_bit(int);
	void update_sub(int);

	static Modulator* Instance();

	std::ostream& out();
	
private:
	float calc_energy(bool);

	char active_byte = 0x0;
	bool sending_bit = 0x0;
    bool first_sub = true;
	std::stringstream data_stream;
};

}
