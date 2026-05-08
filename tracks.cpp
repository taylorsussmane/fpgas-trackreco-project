// #include "tracks.h"
#include <stdio.h>
#include <iostream>
#include "dbscan.h"
#include <fstream>
#include <vector>
#include <sstream>
#include <algorithm> 
#include "kalmanFilter.cpp"
#include <Eigen/Dense>
#include <set> 


std::vector<Point> read_data(){
    Point point;
    int clusterID = -1;
    point.clusterID = clusterID;
    std::ifstream file("trackerInputs_SingleHitEvents.csv");
    std::string line;
    std::vector<Point> pointVec;
    /* Already defined in dbscan.h
    struct Point_ {
        float x, y, t;
	int eventID, clusterID;
    }Point;
    */

#pragma HLS interface port=ap_memory pointVec

    if (file.is_open()) {
        std::getline(file, line); // Read the header line (first line) and do nothing with it

        while (std::getline(file,line)) {
	#pragma HLS loop_flatten 
            std::stringstream ss(line);
	    std::string eventIDStr;
	    std::string layerIDStr;
            std::string xStr;
            std::string yStr;
            std::string zStr;
            std::string tStr;

            std::getline(ss, eventIDStr, ',');
            std::getline(ss, layerIDStr, ',');
            std::getline(ss, xStr, ',');
            std::getline(ss, yStr, ',');
            std::getline(ss, zStr, ',');
            std::getline(ss, tStr, ',');

            point.x = std::stof(xStr);
            point.y = std::stof(yStr);
            point.z = std::stof(zStr);
            point.t = std::stof(tStr);
            point.eventID = std::stoi(eventIDStr);
       
            pointVec.push_back(point);
        }
        std::cout << "Size of vector of Point point = " << pointVec.size() << std::endl;
    }
    return pointVec;
}

int DBSCAN::run()
{
    int clusterID = 1;
    std::vector<Point>::iterator iter;
    for(iter = m_points.begin(); iter != m_points.end(); ++iter){
	#pragma HLS loop_flatten
        if ( iter->clusterID == UNCLASSIFIED ){
            if ( expandCluster(*iter, clusterID) != FAILURE ){
                clusterID += 1;
            }
        }
    }

    return 0;
}

int DBSCAN::expandCluster(Point point, int clusterID) {
    std::vector<int> clusterSeeds = calculateCluster(point);
    if ( clusterSeeds.size() < m_minPoints ) {
        point.clusterID = NOISE;
        return FAILURE;
    }
    else {
        int index = 0, indexCorePoint = 0;
        std::vector<int>::iterator iterSeeds;
        for( iterSeeds = clusterSeeds.begin(); iterSeeds != clusterSeeds.end(); ++iterSeeds){
	    #pragma HLS loop_flatten
            m_points.at(*iterSeeds).clusterID = clusterID;
            if (m_points.at(*iterSeeds).x == point.x && m_points.at(*iterSeeds).y == point.y && m_points.at(*iterSeeds).z == point.z  && m_points.at(*iterSeeds).t == point.t) {
                indexCorePoint = index;
            }
            ++index;
        }
        clusterSeeds.erase(clusterSeeds.begin()+indexCorePoint);

        for(std::vector<int>::size_type i = 0, n = clusterSeeds.size(); i < n; ++i ) {
            #pragma HLS loop_flatten
	    std::vector<int> clusterNeighors = calculateCluster(m_points.at(clusterSeeds[i]));

            if ( clusterNeighors.size() >= m_minPoints ) {
                std::vector<int>::iterator iterNeighors;
                for ( iterNeighors = clusterNeighors.begin(); iterNeighors != clusterNeighors.end(); ++iterNeighors ) {
		    #pragma HLS loop_flatten
                    if ( m_points.at(*iterNeighors).clusterID == UNCLASSIFIED || m_points.at(*iterNeighors).clusterID == NOISE ) {
                        if ( m_points.at(*iterNeighors).clusterID == UNCLASSIFIED ) {
                            clusterSeeds.push_back(*iterNeighors);
                            n = clusterSeeds.size();
                        }
                        m_points.at(*iterNeighors).clusterID = clusterID;
                    }
                }
            }
        }
        return SUCCESS;
    }
}

std::vector<int> DBSCAN::calculateCluster(Point point) {
    int index = 0;
    std::vector<Point>::iterator iter;
    std::vector<int> clusterIndex;
    for( iter = m_points.begin(); iter != m_points.end(); ++iter) {
        #pragma HLS loop_flatten
        if ( calculateDistance(point, *iter) <= m_epsilon && calculateTime(point, *iter) <= 1) { // Hard code 1 ig 
            clusterIndex.push_back(index);
        }
        index++;
    }
    return clusterIndex;
}

inline double DBSCAN::calculateTime(const Point& pointCore, const Point& pointTarget ) {
    return pow(pointCore.t - pointTarget.t,2);
}

inline double DBSCAN::calculateDistance(const Point& pointCore, const Point& pointTarget ) {
    return pow(pointCore.x - pointTarget.x,2)+pow(pointCore.y - pointTarget.y,2)+pow(pointCore.z - pointTarget.z,2);
}

std::vector<Point> sort_objects(std::vector<Point>& point){
    int N = point.size();
    for (int i = 0; i < N - 1; i++){
        #pragma HLS PIPELINE II=2
        for (int j = 0; j < N - i - 1; j++) {
            if (point[j].clusterID > point[j + 1].clusterID){
	        std::swap(point[j], point[j + 1]);
            }
	}
    }
    return point;
}

void printResults(std::vector<Point>& points, int num_points) {
    int i = 0;
    printf("Number of points: %u\n"
    " x     y     z     t     cluster_id    event_id \n"
    "-----------------------------\n"
    , num_points);
    while (i < num_points){
        #pragma HLS loop_flatten
        printf("%5.2lf %5.2lf %5.2lf %5.21f: %d %d\n",
        points[i].x,
        points[i].y, points[i].z, points[i].t,
        points[i].clusterID);
	points[i].eventID;
        ++i;
    }
}

void push_back(Eigen::VectorXd& vec, const float& val){
	auto n = vec.size();
	vec.conservativeResize(n+1);
	vec[n] = val;
}

trackerInput restructure(std::vector<Point> point) {
    trackerInput input;
    for (int i = 0; i < point.size(); i++) {
        #pragma HLS loop_flatten
        push_back(input.x, point[i].x);
        push_back(input.y, point[i].y);
        push_back(input.z, point[i].z);
        push_back(input.t, point[i].t);
        push_back(input.eventID, point[i].eventID);
        push_back(input.clusterID, point[i].clusterID);
    }
    return input;
}

trackerInput tracker_inputs(const trackerInput& input) {
    trackerInput updated_input;
    // Get unique cluster IDs
    std::set<int> uniqueIDs(input.clusterID.begin(), input.clusterID.end());

    // Loop over each unique cluster ID
    for (int id : uniqueIDs) {
        #pragma HLS loop_flatten
        float x_min = 0, x_max = 0;
        float y_min = 0, y_max = 0;
        float z_min = 0, z_max = 0;
        float t_min = 0, t_max = 0;

        bool firstMatch = true;

        // Search through original vectors
        for (size_t i = 0; i < input.clusterID.size(); i++) {
            #pragma HLS loop_flatten
	    if (input.clusterID[i] == id) {
                float x = input.x[i];
                float y = input.y[i];
                float z = input.z[i];
                float t = input.t[i];
                // Initialize mins/maxes
                if (firstMatch) {
                    x_min = x_max = x;
                    y_min = y_max = y;
                    z_min = z_max = z;
                    t_min = t_max = t;
                    firstMatch = false;
                }
                else {
                    x_min = std::min(x_min, x);
                    x_max = std::max(x_max, x);
                    y_min = std::min(y_min, y);
                    y_max = std::max(y_max, y);
                    z_min = std::min(z_min, z);
                    z_max = std::max(z_max, z);
                    t_min = std::min(t_min, t);
                    t_max = std::max(t_max, t);
                }
            }
        }

        // Fill updated struct
        updated_input.clusterID.push_back(id);

        // Midpoints
        updated_input.x.push_back( (x_max + x_min) / 2.0 );
        updated_input.y.push_back( (y_max + y_min) / 2.0 );
        updated_input.z.push_back( (z_max + z_min) / 2.0 );
        updated_input.t.push_back( (t_max - t_min) );

        // Half widths
        updated_input.dx.push_back( (x_max - x_min) / 2.0 );
        updated_input.dy.push_back( (y_max - y_min) / 2.0 );
        updated_input.dz.push_back( (z_max - z_min) / 2.0 );
    }

    return updated_input;
}

/*
trackerInput tracker_inputs(trackerInput input) {
    trackerInput updated_input;
    // Clean up the code by renaming the vectors within the struct
    // Simplify the cluserID vector by getting rid of redundancies
    std::set<int> clusterIDVal(input.clusterID.begin(), input.clusterID.end());
    */ /*
    std::cout << "unique clusterIDVal: " ;
    for (int x : clusterIDVal) {
        std:: cout << x << " ";
    }
    std:: cout << std::endl;
    */ /*
    std::cout << "unique clusterIDVal.size(): " << clusterIDVal.size() << std::endl;

    std::vector<float> xVal = input.x;
    std::vector<float> dxVal = input.dx;
    std::vector<float> yVal = input.y;
    std::vector<float> dyVal = input.dy;
    std::vector<float> zVal = input.z;
    std::vector<float> dzVal = input.dz;
    
    for (int i = 0; i < input.clusterID.size(); i++) {
        int j = 1;
        while (clusterIDVal[i] == j) {
            #pragma HLS loop_flatten
	    x_max = std::max_element(point[i].clusterID.begin(), point[i].clusterID.end()) // This is wrong
            x_min
            y_max
            y_min
            z_max
            z_min
	    j++;
        }
    }
    return updated_input
}
*/



int main() {
    #pragma HLS dataflow

    std::vector<Point> point;
    std::cout << "Checking if things compile" << std::endl;
    point = read_data();
    std::cout << "read_data() function works" << std::endl;

    std::cout << "Checking DBSCAN function" << std::endl;
    DBSCAN ds(MINIMUM_POINTS, EPSILON, point);
    ds.run();
    std::cout << "Clustering algorithm runs" << std::endl;
    // printResults(ds.m_points, ds.getTotalPointSize());
    // printResults(ds.m_points, 20);

    std::cout << "Checking swap/sort functions" << std::endl;
    std::vector<Point> sorted_points;
    sorted_points = sort_objects(ds.m_points);
    printResults(sorted_points, 20);//ds.getTotalPointSize());
    std::cout << "Sorting worked" << std::endl;

    std::cout << "Reformat so that we get vectors of each struct variable" << std::endl;
    trackerInput inputs;
    inputs = restructure(sorted_points);
    // std::cout << "inputs.x = " << inputs.x << std::endl;
    std::cout << "Now we have a struct of vectors for tracking identification" << std::endl;

    tracker_inputs(inputs);
	
	//prepare matrices for kalman filter
	int max = inputs.x.size();
	int m = 3; //number of states
	int n = 5; //number of measurements
	
	for (int i = 0; i < max; i+5){
		#pragma HLS LOOP_FLATTEN
		Eigen::MatrixXd A(n,n); //system dynamics
		Eigen::MatrixXd C(m,n); //output
		Eigen::MatrixXd Q(n,n); //process noise covariance
		Eigen::MatrixXd R(m,m); //measurement noise covariance
		Eigen::MatrixXd P(n,n); //estimate errror covariance 
		A << 1,0,0,0,0 , 0,1,0,0,0 , 0,0,1,0,0 , 0,0,0,1,0 , 0,0,0,0,1;
		C.setZero();
		Q << .05,.05,.05,.05,.05 , .05,.05,.05,.05,.05 , .05,.05,.05,.05,.05 , .05,.05,.05,.05,.05 , .05,.05,.05,.05,.05;
		R << 5;
		P << pow(inputs.dx(i)*inputs.dz(i)*inputs.dy(i),2),inputs.dx(i)*inputs.dz(i)*inputs.dy(i),inputs.dx(i)*inputs.dz(i)*inputs.dy(i),inputs.dx(i)*inputs.dz(i)*inputs.dy(i),
			pow(inputs.dx(i+1)*inputs.dz(i+1)*inputs.dy(i+1),2),inputs.dx(i+1)*inputs.dz(i+1)*inputs.dy(i+1),inputs.dx(i+1)*inputs.dz(i+1)*inputs.dy(i+1),inputs.dx(i+1)*inputs.dz(i+1)*inputs.dy(i+1),
			pow(inputs.dx(i+2)*inputs.dz(i+2)*inputs.dy(i+2),2),inputs.dx(i+2)*inputs.dz(i+2)*inputs.dy(i+2),inputs.dx(i+2)*inputs.dz(i+2)*inputs.dy(i+2),inputs.dx(i+2)*inputs.dz(i+2)*inputs.dy(i+2),
			pow(inputs.dx(i+3)*inputs.dz(i+3)*inputs.dy(i+3),2),inputs.dx(i+3)*inputs.dz(i+3)*inputs.dy(i+3),inputs.dx(i+3)*inputs.dz(i+3)*inputs.dy(i+3),inputs.dx(i+3)*inputs.dz(i+3)*inputs.dy(i+3),
			pow(inputs.dx(i+4)*inputs.dz(i+4)*inputs.dy(i+4),2),inputs.dx(i+4)*inputs.dz(i+4)*inputs.dy(i+4),inputs.dx(i+4)*inputs.dz(i+4)*inputs.dy(i+4),inputs.dx(i+4)*inputs.dz(i+4)*inputs.dy(i+4);

		KalmanFilter kf(inputs.dt(i), A, C, Q, R, P);
		kf.init(inputs.t(i), inputs.x);
		
		Eigen::VectorXd y(m);
		y << inputs.x(i);
		
		kf.update(y);
		std::cout << "t = " << inputs.t(i) << ", " << "y[" << i << "] = " << y.transpose()
        << ", x_hat[" << i << "] = " << kf.state().transpose() << std::endl;
	}
	
    return 0;
}

/*
void tracks(dint16_t A[N], dint16_t B[N], dint16_t C[N]){

read_data;
clustering;
kalman;

return

}
*/
