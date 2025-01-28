#ifndef R_SETTINGS_HPP
#define R_SETTINGS_HPP

#include "CamadaEnlace.hpp"
#include "CamadaFisica.hpp"

struct R_Settings {
    float frequency = 10;
    unsigned resolution = 10;
    enum MODS { NRZ_POLAR, MANCHESTER, BIPOLAR};
    MODS modulation = NRZ_POLAR;
    FRAMING framing = BYTE_COUNT;
    ERROR err_handling = NONE;
}typedef R_Settings;

#endif // R_SETTINGS_HPP
