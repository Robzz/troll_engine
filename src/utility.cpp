#include "utility.h"
#include <cmath>

namespace Engine {

float deg_to_rad(float angle) {
    return angle * M_PI / 180.f;
}

float rad_to_deg(float angle) {
    return angle * 180.f / M_PI;
}

} // namespace Engine
