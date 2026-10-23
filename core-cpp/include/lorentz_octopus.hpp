#pragma once

#include <array>
#include "integrator.hpp"
#include "parker_spiral.hpp"

namespace octo {

    constexpr std::size_t kArmCount    = 8;
    constexpr std::size_t kArmSegments = 16;

    struct ArmResult {
            double current_exa_amp{};
            double force_newton{};
            double root_bending_stress_pa{};
            double safety_factor{};
            double hinge_angle_rad{};
    };

    struct OctopusResult {
            std::array<ArmResult, kArmCount> arms{};
            Vec3 total_force;
            Vec3 total_torque;
            double minimum_safety_factor{};
            double maximum_current_density{};
            double critical_current_density{};
    };

    struct OctopusModel {
            double arm_length_m{1.2e7};
            double conductor_area_m2{0.015};
            double critical_current_density_a_m2{1.0e20};
            double yield_strength_pa{1.5e18};
            double arm_width_m{1.0e4};
            double arm_thickness_m{1.0e4};

            OctopusResult evaluate (const Vec3 &position_m, double time_seconds, double requested_force_newton,
                                    const ParkerSpiral &parker = {}) const;
    };

} // namespace octo
