#ifndef T_SETTINGS_HPP
#define T_SETTINGS_HPP

struct T_Settings {
    float frequency = 10;
    unsigned resolution = 4;
    float error_chance = 0;
    enum MODS { NRZ_POLAR, MANCHESTER , BIPOLAR, _ASK, _FSK, _4_QAM};
    MODS modulation = NRZ_POLAR;
    enum FRAMES { BYTE_COUNT, BYTE_INSERT };
    FRAMES framing = BYTE_COUNT;
    enum ERRORS { NONE, PARITY, CRC, HAMMING };
    ERRORS err_handling = NONE;
}typedef T_Settings;

#endif // T_SETTINGS_HPP
