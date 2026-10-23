#include "integrator.hpp"
#include <algorithm>
#include <cmath>

namespace octo {
    Vec3 &Vec3::operator+= (const Vec3 &v) {
        x += v.x;
        y += v.y;
        z += v.z;
        return *this;
    }
    Vec3 &Vec3::operator-= (const Vec3 &v) {
        x -= v.x;
        y -= v.y;
        z -= v.z;
        return *this;
    }
    Vec3 &Vec3::operator*= (double s) {
        x *= s;
        y *= s;
        z *= s;
        return *this;
    }
    Vec3 operator+ (Vec3 a, const Vec3 &b) { return a += b; }
    Vec3 operator- (Vec3 a, const Vec3 &b) { return a -= b; }
    Vec3 operator- (const Vec3 &v) { return {-v.x, -v.y, -v.z}; }
    Vec3 operator* (Vec3 v, double s) { return v *= s; }
    Vec3 operator* (double s, Vec3 v) { return v *= s; }
    double dot (const Vec3 &a, const Vec3 &b) { return a.x * b.x + a.y * b.y + a.z * b.z; }
    double norm (const Vec3 &v) { return std::sqrt (dot (v, v)); }

    static Vec3 acceleration (const Vec3 &p, double mu) {
        const double r2 = dot (p, p);
        return r2 > 0.0 ? p * (-mu / (r2 * std::sqrt (r2))) : Vec3{};
    }
    static void verlet (OrbitalState &s, double h, double mu) {
        s.position_m += s.velocity_m_s * (0.5 * h);
        s.velocity_m_s += acceleration (s.position_m, mu) * h;
        s.position_m += s.velocity_m_s * (0.5 * h);
    }
    double specificOrbitalEnergy (const OrbitalState &s, double mu) {
        return 0.5 * dot (s.velocity_m_s, s.velocity_m_s) - mu / norm (s.position_m);
    }
    void yoshidaStep (OrbitalState &s, double dt, double mu) {
        constexpr double cbrt2 = 1.2599210498948731648;
        const double w1        = 1.0 / (2.0 - cbrt2);
        const double w0        = -cbrt2 / (2.0 - cbrt2);
        verlet (s, w1 * dt, mu);
        verlet (s, w0 * dt, mu);
        verlet (s, w1 * dt, mu);
    }
    void integrate (OrbitalState &s, double duration, double step, double mu) {
        if (!(duration > 0.0) || !(step > 0.0) || !(mu > 0.0)) return;
        const std::size_t count = static_cast<std::size_t> (std::ceil (duration / step));
        for (std::size_t i = 0; i < count; ++i) {
            const double remaining = std::max (0.0, duration - static_cast<double> (i) * step);
            if (remaining > 0.0) yoshidaStep (s, std::min (step, remaining), mu);
        }
    }
} // namespace octo
