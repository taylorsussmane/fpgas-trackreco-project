# TAC-HEP FPGAs track reconstruction project

**1. Input Simulation [Kyla]**
- Take simulated tracker hits
	- Simulated data arrives in a csv file of (event_id, layer_id, x, y, z, time) provided by Varun
	- To provide ease of validating code, the provided .csv file was simplified to simulate only one track per event
	- The intention was to ensure the framework could sufficiently meet expectations before extrapolating to more complicated event tracking

**2. Clustering Algorithm [Kayleigh]**
- Group nearby hits in the same detector layer to form cluster candidates (e.g., based on
distance or time)
	- Implement the DBSCAN algorithm 
	- Originally pulled from [james-yoo](https://github.com/james-yoo/DBSCAN/blob/master/README.md) with further edits)
	- Restructure the data output from struct -> 

**3. Sorting Hits [Kayleigh/Kyla]**
- Sort hits by layer number, timestamp, or X/Y position to prepare for track fitting

**4. Track Fitting using Kalman Filter [Taylor]**
- Apply a Kalman filter to clustered and sorted hits to estimate track parameters (position,
momentum, angle)
	- Originally pulled from [hmartiro](https://github.com/hmartiro/kalman-cpp) with further edits

**5. Send tracks to output**
