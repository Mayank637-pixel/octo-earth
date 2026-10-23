#include "lorentz_octopus.hpp"
#include <algorithm>
#include <cmath>
#include <limits>

namespace octo {
    static constexpr double kPi = 3.14159265358979323846;
    MagneticField ParkerSpiral::field (const Vec3 &p, double) const {
        const double r      = std::max (norm (p), kAstronomicalUnitM * 1e-9);
        const double lat    = std::asin (std::clamp (p.z / r, -1.0, 1.0));
        const double lon    = std::atan2 (p.y, p.x);
        const double radial = field_at_one_au_t * kAstronomicalUnitM * kAstronomicalUnitM / (r * r);
        const double spiral = solar_rotation_rad_s * r / solar_wind_m_s;
        const double clat   = std::cos (lat);
        return {{radial * (clat * std::cos (lon) + spiral * std::sin (lon)),
                 radial * (clat * std::sin (lon) - spiral * std::cos (lon)), radial * std::sin (lat)}};
    }
    OctopusResult OctopusModel::evaluate (const Vec3 &position, double time, double requested_force,
                                          const ParkerSpiral &parker) const {
        OctopusResult out;
        const double b               = std::max (norm (parker.field (position, time).tesla), 1e-30);
        const double per_arm         = std::max (0.0, requested_force) / static_cast<double> (kArmCount);
        const double current         = per_arm / (arm_length_m * b);
        out.maximum_current_density  = current / std::max (conductor_area_m2, 1e-30);
        out.critical_current_density = critical_current_density_a_m2;
        out.minimum_safety_factor    = std::numeric_limits<double>::infinity ();
        for (std::size_t k = 0; k < kArmCount; ++k) {
            const double phase         = 2.0 * kPi * static_cast<double> (k) / kArmCount;
            const double hinge         = 0.05 * std::sin (time / kSecondsPerYear * 2.0 * kPi + phase);
            const double moment        = per_arm * arm_length_m * 0.5 * std::cos (hinge);
            const double inertia       = arm_width_m * std::pow (std::max (arm_thickness_m, 1e-12), 3) / 12.0;
            auto &arm                  = out.arms[k];
            arm.current_exa_amp        = current / 1e18;
            arm.force_newton           = per_arm;
            arm.root_bending_stress_pa = moment * (arm_thickness_m * 0.5) / std::max (inertia, 1e-30);
            arm.safety_factor          = yield_strength_pa / std::max (arm.root_bending_stress_pa, 1e-30);
            arm.hinge_angle_rad        = hinge;
            out.minimum_safety_factor  = std::min (out.minimum_safety_factor, arm.safety_factor);
            const Vec3 tangent{-std::sin (phase), std::cos (phase), 0.0};
            out.total_force += tangent * per_arm;
        }
        return out;
    }
} // namespace octo
