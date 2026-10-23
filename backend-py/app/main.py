from fastapi import FastAPI, WebSocket, WebSocketDisconnect
from fastapi.middleware.cors import CORSMiddleware
import numpy as np
from app.core_loader import TELEMETRY_DTYPE, SimConfig, load_octocore

app = FastAPI(title="Octo-Earth Mission Control API")

app.add_middleware(
    CORSMiddleware,
    allow_origins=["*"],
    allow_credentials=True,
    allow_methods=["*"],
    allow_headers=["*"],
)

libocto = load_octocore()

@app.get("/health")
def health():
    return {"status": "ok", "core_loaded": libocto is not None}

@app.websocket("/ws/simulate")
async def websocket_simulate(websocket: WebSocket):
    await websocket.accept()
    frames_count = 10000
    telemetry_buffer = np.zeros(frames_count, dtype=TELEMETRY_DTYPE)

    if libocto:
        cfg = SimConfig(
            target_au=2.70,
            max_years=10000.0,
            arm_length_m=1.2e7,
            jc_limit_a_m2=1.0e20,
            yield_strength_pa=1.5e18,
            gravity_tether_on=1,
            num_output_frames=frames_count,
        )
        libocto.run_simulation(
            cfg,
            telemetry_buffer.ctypes.data,
            frames_count,
        )

    try:
        # Stream raw bytes in chunks of 100 frames (17.6 KB)
        chunk_size = 100
        for i in range(0, frames_count, chunk_size):
            chunk = telemetry_buffer[i : i + chunk_size]
            await websocket.send_bytes(chunk.tobytes())
    except WebSocketDisconnect:
        pass