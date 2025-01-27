#ifndef R_SETTINGS_HPP
#define R_SETTINGS_HPP

struct R_Settings {
    float frequency = 10;
    unsigned resolution = 4;
    enum MODS { NRZ_POLAR, MANCHESTER, BIPOLAR};
    MODS modulation = NRZ_POLAR;
}typedef R_Settings;

#endif // R_SETTINGS_HPP
