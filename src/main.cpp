#include <Geode/Geode.hpp>
#include <Geode/modify/CCScheduler.hpp>
#include <cstdlib>
#include <ctime>

using namespace geode::prelude;

// gonna be using this a lot and I don't wanna type the whole thing out every time
#define listenToSettingC(type, key, capture, alterValue) listenForSettingChanges<type>(key, [capture](type value) { *alterValue = value; } )
#define listenToSetting(type, key, alterValue) listenForSettingChanges<type>(key, [](type value) { *alterValue = value; } )

// I really shouldn't be using global variables but I can't add fields to non-ccnode hooks, and it also won't let me add regular members, so I kinda have no choice
bool speedChangeEnabled;
bool speedChangeAudioOnly;
float speedChangeFrequency;
float speedChangeIntensity;
float totalDelta = 0.0f;
float currentSpeed = 0.0f;
FMOD::ChannelGroup* masterChannel;

class $modify(DrunkCCScheduler, CCScheduler) {
    void update(float dt) {
        // get audio channel if it's not been obtained yet
        if (!masterChannel) {
            FMODAudioEngine::sharedEngine()->m_system->getMasterChannelGroup(&masterChannel);
        }

        if (speedChangeEnabled) {
            totalDelta += dt;
            if (totalDelta >= speedChangeFrequency) {
                // using srand and rand since we don't really need a secure or "good" random number generator
                const auto val = static_cast<float>(std::rand())/static_cast<float>(RAND_MAX); // random number from 0 to 1
                const auto mul = (val * (speedChangeIntensity * 2.0f)) - speedChangeIntensity; // get the number to be from (1 - intensity) to (1 + intensity)
                currentSpeed = 1.0f - mul;
                totalDelta = 0;
            }

            masterChannel->setPitch(currentSpeed);
        } else {
            masterChannel->setPitch(1.0f);
        }

        // if speed change is disabled OR audio only is enabled, supply regular dt. otherwise, use the modified one
        CCScheduler::update(
            speedChangeEnabled ?
            (speedChangeAudioOnly ? dt : dt * currentSpeed) : dt);
    }
};

$on_mod(Loaded) {
    std::srand(static_cast<unsigned int>(std::time(nullptr)));

    speedChangeEnabled = Mod::get<>()->getSettingValue<bool>("hangover-enabled");
    speedChangeAudioOnly = Mod::get<>()->getSettingValue<bool>("hangover-speedchange-audio-only");
    speedChangeFrequency = Mod::get<>()->getSettingValue<float>("hangover-speedchange-frequency");
    speedChangeIntensity = Mod::get<>()->getSettingValue<float>("hangover-speedchange-intensity");

    listenToSetting(bool, "hangover-enabled", &speedChangeEnabled);
    listenToSetting(bool, "hangover-speedchange-audio-only", &speedChangeAudioOnly);
    listenToSetting(float, "hangover-speedchange-frequency", &speedChangeFrequency);
    listenToSetting(float, "hangover-speedchange-intensity", &speedChangeIntensity);
}