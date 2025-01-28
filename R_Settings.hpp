#ifndef R_SETTINGS_HPP
#define R_SETTINGS_HPP

#include "CamadaEnlace.hpp"
#include "CamadaFisica.hpp"

struct R_Settings {
    float frequency = 10;
    unsigned resolution = 10;
    enum MODS { NRZ_POLAR, MANCHESTER, BIPOLAR};
    MODS modulation = NRZ_POLAR;
    enum FRAMES { BYTE_COUNT, BYTE_INSERT };
    FRAMES framing = BYTE_COUNT;
    enum ERRORS { NONE, PARITY, CRC, HAMMING };
    ERRORS err_handling = NONE;
}typedef R_Settings;

#endif // R_SETTINGS_HPP
