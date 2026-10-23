#pragma once

#include "integrator.hpp"

namespace octo {

    struct MagneticField {
            Vec3 tesla;
    };

    struct ParkerSpiral {
            double field_at_one_au_t{5.0e-9};
            double solar_rotation_rad_s{2.86533e-6};
            double solar_wind_m_s{400'000.0};

            MagneticField field (const Vec3 &position_m, double time_seconds = 0.0) const;
    };

} // namespace octo
