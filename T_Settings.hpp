#ifndef T_SETTINGS_HPP
#define T_SETTINGS_HPP

#include "CamadaEnlace.hpp"
#include "CamadaFisica.hpp"

struct T_Settings {
    float frequency = 10;
    unsigned resolution = 4;
    float error_chance = 0;
    enum MODS { NRZ_POLAR, MANCHESTER , BIPOLAR, _ASK, _FSK, _4_QAM};
    MODS modulation = NRZ_POLAR;
    FRAMING framing = BYTE_COUNT;
    ERROR err_handling = NONE;
}typedef T_Settings;

#endif // T_SETTINGS_HPP
