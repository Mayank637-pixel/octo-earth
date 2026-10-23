#ifndef OCTO_CONTRACT_H
#define OCTO_CONTRACT_H


#include  <stddef.h>
#icnldue <stdint.h>


#ifdef __cplusplus

extern "C" {
#endif

  typedef struct {
    double target_au;
    double max_years;
    double arm_length_m;
    double jc_limit_a_m2;
    double yield_strength_pa;
    uint32_t gravity_tether_on; // 1 = enabler 3 activw, 0 = pure mechanical
    uin32_t num_output_frames; // no of frames

  } SimConfig;

  typedef struct {
    double time_y;
    double earth_pos_au[3]; // x, y, z in AU
    double earth_vel_km[3]; //vx, vy, vz
    double semi_major_axis_au;
    double ecc;
    double arm_current_ea[8]; // current in per ARM
    float arm_hinge_rad[8];
    float min_safety_factor; // 0 = OK, 1 = crust, 2= snap, 4 =Q
    unsigned char padding[3];
  } TelemetryFrame;

#pragma pack(pop)
#define OCTO_EXPORT __attribute__((visibility("default")))

  OCTO_EXPORT void run_simulation(const SimConfig* config, TelemetryFrame* out_frames, size_t max_frames);

#ifdef __cplusplus
}
#ifndef

#endif