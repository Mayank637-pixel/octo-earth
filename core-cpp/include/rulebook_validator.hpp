#pragma once

#include <cstdint>
#include "lorentz_octopus.hpp"

namespace octo {

    enum class Status : std::uint8_t { Ok = 0, CrustShear = 1, ArmSnap = 2, Quench = 4 };

    std::uint8_t validateRulebook (const OctopusResult &result, bool gravity_tether_on, double surface_delta_g = 0.0);

} // namespace octo
