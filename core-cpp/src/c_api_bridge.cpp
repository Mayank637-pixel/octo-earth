#include  "contract.h"
#include  <cmath>
#include  <cstring>
using namespace std;

extern "C" {
  OCTO_EXPORT void run_simulation(const SimConfig* config, TelemetryFrame* out_frames, size_t max_frames) {
    if (!config || !out_frames || !max_frames ) return;

    const double r_start = 1.0;
    const double r_end = config->target_au > 1.0 ? config->target_au : 2.7;
    const double total_years = config->max_years > 0.0 ? config->max_years :10000.0;
    const double delta_r = r_end - r_start;

    for (size_t i = 0; i < max_frames; ++i) {
      double progress = static_cast<double>(i) / static_cast<double>(max_frames-1);
      double t_years = progress * total_years;
      double r = r_start + delta_r * progress;

      double v = 29.78 / sqrt(r);
      double theta = progress * 200.0 * 2.0 *M_PI;

      TelemetryFrame& frame  = out_frames[i];
      frame.time_y  =t_years;
      frame.earth_pos_au[0] = r * cos(theta);
      frame.earth_pos_au[1] = r * sin(theta);
      frame.earth_pos_au[2] = 0.0;

      frame.earth_vel_km[0] = -v * sin(theta);
      frame.earth_vel_km[1] = v * cos(theta);
      frame.earth_vel_km[2] = 0.0;

      frame.semi_major_axis_au  = r;
      frame.ecc = 0.0167 * (1.0 - 0.5*progress);
      for (int k = 0; k < 8; k++) {
        frame.arm_current_ea[k] = 0.78 + 0.05*sin(progress  * 100.0 + k);
        frame.arm_hinge_rad[k] = (float) (0.15 * sin(progress  * 50.0 + k));
      }

      frame.min_safety_factor = (config -> yield_strength_pa >= 1.0e18) ? 1.85f : 0.45f;
      frame.status_bitmask = (frame.min_safety_factor < 1.5f) ? 0x02 : 0x00;
      frame.padding[0]= frame.padding[1] = frame.padding[2] = 0;

    }
  }




}