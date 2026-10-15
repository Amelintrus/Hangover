#include "../header/Config.hpp"
#include <Geode/modify/CCScheduler.hpp>
#include <cstdlib>

using namespace geode::prelude;

namespace hangover {
    float totalDelta = 0.0f;
    float currentSpeed = 0.0f;
    FMOD::ChannelGroup* masterChannel;

    class $modify(HangoverCCScheduler, CCScheduler) {
        void update(float dt) {
            // get audio channel if it's not been obtained yet
            if (!masterChannel) {
                FMODAudioEngine::sharedEngine()->m_system->getMasterChannelGroup(&masterChannel);
            }

            if (Config::hangoverEnabled && Config::speedChangeEnabled) {
                totalDelta += dt;
                if (totalDelta >= Config::speedChangeFrequency) {
                    // using srand and rand since we don't really need a secure or "good" random number generator
                    const auto val = static_cast<float>(std::rand())/static_cast<float>(RAND_MAX); // random number from 0 to 1
                    const auto mul = (val * (Config::speedChangeIntensity * 2.0f)) - Config::speedChangeIntensity; // get the number to be from (1 - intensity) to (1 + intensity)
                    currentSpeed = 1.0f - mul;
                    totalDelta = 0;
                }

                masterChannel->setPitch(currentSpeed);
            } else {
                masterChannel->setPitch(1.0f);
            }

            // if speed change is disabled OR audio only is enabled, supply regular dt. otherwise, use the modified one
            CCScheduler::update(
                Config::hangoverEnabled && Config::speedChangeEnabled ?
                (Config::speedChangeAudioOnly ? dt : dt * currentSpeed) : dt);
        }
    };
}