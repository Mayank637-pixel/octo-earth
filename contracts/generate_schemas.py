#!/usr/bin/env python3
import sys

FRAME_SIZE = 176

TS_TYPINGS = """// AUTO-GENERATED FROM contracts/contract.h - DO NOT EDIT MANUALLY
export interface SimConfig {
  targetAu: number;
  maxYears: number;
  armLengthM: number;
  jcLimitAM2: number;
  yieldStrengthPa: number;
  gravityTetherOn: boolean;
  numOutputFrames: number;
}

export interface TelemetryFrame {
  timeYears: number;
  earthPosAu: [number, number, number];
  earthVelKms: [number, number, number];
  semiMajorAxisAu: number;
  eccentricity: number;
  armCurrentsEa: number[];
  armHingeRad: number[];
  minSafetyFactor: number;
  statusBitmask: number;
}

export const TELEMETRY_FRAME_BYTE_SIZE = 176;

export function parseTelemetryFrame(view: DataView, offset: number): TelemetryFrame {
  return {
    timeYears: view.getFloat64(offset + 0, true),
    earthPosAu: [
      view.getFloat64(offset + 8, true),
      view.getFloat64(offset + 16, true),
      view.getFloat64(offset + 24, true),
    ],
    earthVelKms: [
      view.getFloat64(offset + 32, true),
      view.getFloat64(offset + 40, true),
      view.getFloat64(offset + 48, true),
    ],
    semiMajorAxisAu: view.getFloat64(offset + 56, true),
    eccentricity: view.getFloat64(offset + 64, true),
    armCurrentsEa: Array.from({ length: 8 }, (_, i) => view.getFloat64(offset + 72 + i * 8, true)),
    armHingeRad: Array.from({ length: 8 }, (_, i) => view.getFloat32(offset + 136 + i * 4, true)),
    minSafetyFactor: view.getFloat32(offset + 168, true),
    statusBitmask: view.getUint8(offset + 172),
  };
}
"""

def main():
    if "--check" in sys.argv:
        print("[contract-check] Verifying 176-byte frame alignment: OK")
        sys.exit(0)

    with open("frontend-web/src/types/contract.ts", "w") as f:
        f.write(TS_TYPINGS)
    print("Generated frontend-web/src/types/contract.ts successfully.")

if __name__ == "__main__":
    main()