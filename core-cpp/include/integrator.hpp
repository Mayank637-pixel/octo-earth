#pragma once

#include <cstddef>

namespace octo {

    struct Vec3 {
            double x{}, y{}, z{};
            constexpr Vec3 () = default;
            constexpr Vec3 (double x_, double y_, double z_) : x (x_), y (y_), z (z_) {}
            Vec3 &operator+= (const Vec3 &v);
            Vec3 &operator-= (const Vec3 &v);
            Vec3 &operator*= (double s);
    };

    Vec3 operator+ (Vec3 a, const Vec3 &b);
    Vec3 operator- (Vec3 a, const Vec3 &b);
    Vec3 operator- (const Vec3 &v);
    Vec3 operator* (Vec3 v, double s);
    Vec3 operator* (double s, Vec3 v);
    double dot (const Vec3 &a, const Vec3 &b);
    double norm (const Vec3 &v);

    struct OrbitalState {
            Vec3 position_m;
            Vec3 velocity_m_s;
    };

    constexpr double kAstronomicalUnitM = 149'597'870'700.0;
    constexpr double kSolarMuM3S2       = 1.32712440018e20;
    constexpr double kSecondsPerYear    = 31'557'600.0;

    double specificOrbitalEnergy (const OrbitalState &state, double gravitational_parameter = kSolarMuM3S2);
    void yoshidaStep (OrbitalState &state, double dt_seconds, double gravitational_parameter = kSolarMuM3S2);
    void integrate (OrbitalState &state, double duration_seconds, double step_seconds,
                    double gravitational_parameter = kSolarMuM3S2);

} // namespace octo
