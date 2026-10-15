#include "../header/Config.hpp"
#include <Geode/modify/PlayLayer.hpp>

using namespace geode::prelude;

namespace hangover {
    class $modify(HangoverPlayLayer, PlayLayer) {
        struct Fields {
            ShaderLayer* shaderLayer;
        };

        void postUpdate(float dt) {
            if (!m_fields->shaderLayer) {
                if (const auto c = this->getChildByID("shader-layer"))
                    m_fields->shaderLayer = typeinfo_cast<ShaderLayer*>(c);
            } else {
                if (Config::hangoverEnabled && Config::vfxEnabled) {
                    if (Config::blurEnabled)
                        m_fields->shaderLayer->triggerRadialBlur(0.0f, Config::blurIntensity, 1.0f, 1.0f,
                            0, 0, 0, false, 0,
                            0, 0.0f, false);
                }
            }

            PlayLayer::postUpdate(dt);
        }

        void onExit() {
            m_fields->shaderLayer = nullptr;
            PlayLayer::onExit();
        }
    };
}
