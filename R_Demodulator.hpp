#pragma once

namespace Receiver {

class Demodulator {
private:
	Demodulator();

public:
	static Demodulator* Instance();

	bool read_bit();
	char read_byte();

	enum MODS { NRZ_POLAR, MANCHESTER, BIPOLAR};
	MODS modulation = NRZ_POLAR;

private:
	bool calc_bit();
	bool NRZ_polar();
	bool manchester();
	bool bipolar();

	float amp = 1;
};

}
