#include "lorentz_octopus.hpp"
#include "rulebook_validator.hpp"
#include <cmath>
#include <cstdlib>
#include <iostream>

namespace {
void require(bool condition, const char* message) {
    if (!condition) {
        std::cerr << "FAIL: " << message << '\n';
        std::exit(EXIT_FAILURE);
    }
}
}

int main() {
    using namespace octo;
    ParkerSpiral parker;
    const Vec3 one_au{kAstronomicalUnitM, 0.0, 0.0};
    const Vec3 two_au{2.0 * kAstronomicalUnitM, 0.0, 0.0};
    const double b_one = norm(parker.field(one_au).tesla);
    const double b_two = norm(parker.field(two_au).tesla);
    require(std::isfinite(b_one) && b_one > 0.0, "Parker field is finite");
    require(b_two < b_one && b_two > 0.15 * b_one, "Parker field has physical radial falloff");

    OctopusModel model;
    const auto result = model.evaluate(one_au, 0.0, 2.25e17, parker);
    require(result.arms.size() == kArmCount, "all eight arms are evaluated");
    require(result.total_force.x != 0.0 || result.total_force.y != 0.0,
            "Lorentz force is non-zero");
    require(std::isfinite(result.minimum_safety_factor), "safety factor is finite");
    require(validateRulebook(result, true) == 0, "nominal enablers pass rulebook");

    OctopusModel unsafe = model;
    unsafe.critical_current_density_a_m2 = 1.0;
    const auto unsafe_result = unsafe.evaluate(one_au, 0.0, 2.25e17, parker);
    require((validateRulebook(unsafe_result, true) & 4U) != 0,
            "over-current condition raises quench status");
}
