# TAC-HEP FPGAs track reconstruction project

Due to the limited time constraint, the following project compiles and runs. However, we believe that the program does not correctly perform a clustering and tracking procedure. 

**1. Input Simulation [Kyla]**
- Take simulated tracker hits
	- Simulated data arrives in a csv file of (event_id, layer_id, x, y, z, time) provided by Varun
	- To provide ease of validating code, the provided .csv file was simplified to simulate only one track per event
	- That single track was then duplicated with a 10% variation to allow for some semblance of a clustering affect to take place
	- The intention was to ensure the framework could sufficiently meet expectations before extrapolating to more complicated event tracking

**2. Clustering Algorithm [Kayleigh/Kyla]**
- Group nearby hits in the same detector layer to form cluster candidates (e.g., based on
distance or time)
	- Implement the DBSCAN algorithm 
	- Originally pulled from [james-yoo](https://github.com/james-yoo/DBSCAN/blob/master/README.md) with further edits)
- Restructure the data output from a vector of structs -> to a struct of vectors 
	- The data was provided as a vector of each hit categorized as a struct each containing hit information such as position (x,y,z), timing, and layer identification
- In every for and while loop we included loop flatten because we weren't partitioning the arrays, so this served to parallelize the loop iterations

**3. Sorting Hits [Kayleigh/Kyla]**
- Sort hits by clusterID to prepare for track fitting
- The data is returned for each cluster as a struct containing Eigen::vectors for each hit information (position, timing, etc)

**4. Track Fitting using Kalman Filter and Send Tracks to Output [Taylor]**
- Apply a Kalman filter to clustered and sorted hits to estimate track parameters (position,
momentum, angle)
	- Originally pulled from [hmartiro](https://github.com/hmartiro/kalman-cpp) with further edits
	- Kalman filter uses Eigen library to do the matrix operations
- The filter is implemented in tracks.cpp
	- First, the covariance matrices were defined for 5 clusters (one in each layer ID) to make a track.
	- The system dynamics, process noise covariance, and measurement noise covariance were defined with constant, arbitrary values. The actual values would depend on the detector and the distribution of the hits.
 	- The estimate error covariance was estimated using the uncertainty in the position measurements.
  	- Finally the Kalman filter was applied to the 5 clusters and the output is printed. The output vector contains only the momentum at the moment. 
 - The pragmas used were loop flatten in tracks.cpp and latency constraint, allocation limit to the transpose operation in kalmanFilter.cpp.
   	- I didn't have enough time to fully optimize the timing and resource utilization, but it could certainly be better.
   	- It also would have been better to define a Matrix class that contains addition, multiplication, transpose, and inverse for arrays, as it would allow for array pragmas to be used. However, I didn't have time to create that class, and instead used the Eigen library which already had those operations implemented. 

**5. Pragmas Included**
- To generate the data, a python script was used so no pragmas were applied 
- The clustering algorithm has many pragmas because there were many different functions created. Some pragmas used include: 
	- HLS interface
	- Loop_flatten
   	- Allocation
   	- Latency
	- Dataflow-- in the main function each subfunction was called, and dataflow was used to optimize how the functions could work on top of each other and to ensure variables weren't being used before the function before was finished.


