#pragma once

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
	
	enum MODS { NRZ_POLAR, MANCHESTER , BIPOLAR, _ASK, _FSK, _8_QAM};
	MODS modulation = NRZ_POLAR;

private:
	float calc_energy(bool);
	float NRZ_polar(bool);
	float manchester(bool);
	float bipolar(bool);
	float amp_shift_key(bool);
	float freq_shift_key(bool);
	float eight_quadrature(bool);

	char active_byte = 0x0;
	bool sending_bit = 0x0;
	std::stringstream data_stream;
	float amp = 1;
	int bipolar_prev_one = 1;
};

}
