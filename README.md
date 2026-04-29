# TAC-HEP FPGAs track reconstruction project

**1. Input Simulation**
- Take simulated tracker hits
  - (layer_id, x, y, z, time, charge) OR (layer_id, r, phi, z, time, charge)

**2. Clustering Algorithm**
- Group nearby hits in the same detector layer to form cluster candidates (e.g., based on
distance or time)
- Clustering can use a simple approach or sliding window logic

**3. Sorting Hits**
- Sort hits by layer number, timestamp, or X/Y position to prepare for track fitting

**4. Track Fitting using Kalman Filter**
- Apply a Kalman filter to clustered and sorted hits to estimate track parameters (position,
momentum, angle)

**5. Send tracks to output**
