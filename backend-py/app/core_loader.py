import ctypes
import os
from pathlib import Path
import numpy as np

TELEMETRY_DTYPE = np.dtype([
    ("time_years", np.float64),
    ("earth_pos_au", np.float64, (3,)),
    ("earth_vel_kms", np.float64, (3,)),
    ("semi_major_axis_au", np.float64),
    ("eccentricity", np.float64),
    ("arm_currents_ea", np.float64, (8,)),
    ("arm_hinge_rad", np.float32, (8,)),
    ("min_safety_factor", np.float32),
    ("status_bitmask", np.uint8),
    ("padding", np.uint8, (3,)),
], align=False)

assert TELEMETRY_DTYPE.itemsize == 176, f"Dtype itemsize {TELEMETRY_DTYPE.itemsize} != 176"

class SimConfig(ctypes.Structure):
    _pack_ = 1
    _fields_ = [
        ("target_au", ctypes.c_double),
        ("max_years", ctypes.c_double),
        ("arm_length_m", ctypes.c_double),
        ("jc_limit_a_m2", ctypes.c_double),
        ("yield_strength_pa", ctypes.c_double),
        ("gravity_tether_on", ctypes.c_uint32),
        ("num_output_frames", ctypes.c_uint32),
    ]

def load_octocore():
    # Search common build paths
    candidates = [
        os.environ.get("OCTOCORE_LIB_PATH", ""),
        str(Path(__file__).resolve().parents[2] / "core-cpp/build/liboctocore.so"),
        str(Path(__file__).resolve().parents[2] / "core-cpp/build/Release/octocore.dll"),
        "./liboctocore.so",
    ]
    for path in candidates:
        if path and os.path.exists(path):
            lib = ctypes.CDLL(path)
            lib.run_simulation.argtypes = [
                ctypes.POINTER(SimConfig),
                ctypes.c_void_p,
                ctypes.c_size_t,
            ]
            lib.run_simulation.restype = None
            return lib
    return None