#include <Geode/Geode.hpp>
#include <Geode/modify/ShaderLayer.hpp>

using namespace geode::prelude;

namespace hangover {
    class $modify(HangoverShaderLayer, ShaderLayer) {
        bool init() {
            if (!ShaderLayer::init()) {
                return false;
            }

            // set the node id so that I can retrieve it for vfx
            this->setID("shader-layer");
            return true;
        }
    };
}
