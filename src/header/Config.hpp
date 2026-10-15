#ifndef HANGOVER_CONFIG_HPP
#define HANGOVER_CONFIG_HPP

#include <Geode/Geode.hpp>
using namespace geode::prelude;

// set the value on mod load AND set the value whenever the config changes
#define hangoverListenToSetting(type, key, alterValue)\
    *alterValue = Mod::get<>()->getSettingValue<type>(key);\
\
    listenForSettingChanges<type>(key, [](type value) {\
        *alterValue = value;\
    });

namespace hangover {
    class Config {
    public:
        inline static bool hangoverEnabled = true;

        // speed change
        inline static bool speedChangeEnabled = true;
        inline static bool speedChangeAudioOnly = false;
        inline static float speedChangeFrequency = 0.4f;
        inline static float speedChangeIntensity = 0.1f;

        // vfx
        inline static bool vfxEnabled = true;
        // radial blur
        inline static bool blurEnabled = true;
        inline static float blurIntensity = 2;
    };
}

using namespace hangover;

$on_mod(Loaded) {
    std::srand(static_cast<unsigned int>(std::time(nullptr)));

    hangoverListenToSetting(bool, "hangover-enabled", &Config::hangoverEnabled)

    hangoverListenToSetting(bool, "hangover-speedchange-enabled", &Config::speedChangeEnabled)
    hangoverListenToSetting(bool, "hangover-speedchange-audio-only", &Config::speedChangeAudioOnly)
    hangoverListenToSetting(float, "hangover-speedchange-frequency", &Config::speedChangeFrequency)
    hangoverListenToSetting(float, "hangover-speedchange-intensity", &Config::speedChangeIntensity)

    hangoverListenToSetting(bool, "hangover-vfx-enabled", &Config::speedChangeEnabled)
    hangoverListenToSetting(bool, "hangover-vfx-blur", &Config::blurEnabled)
    hangoverListenToSetting(float, "hangover-vfx-blur-intensity", &Config::blurIntensity)

    log::info("oughh,,.... getin g hun gover");
}

#endif //HANGOVER_CONFIG_HPP
