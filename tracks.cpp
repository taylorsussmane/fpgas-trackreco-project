// #include "tracks.h"
#include <stdio.h>
#include <iostream>
#include "dbscan.h"
#include <fstream>
#include <vector>
#include <sstream>

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

    if (file.is_open()) {
        std::getline(file, line); // Read the header line (first line) and do nothing with it

        while (std::getline(file,line)) {
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
    for(iter = m_points.begin(); iter != m_points.end(); ++iter)
    {
        if ( iter->clusterID == UNCLASSIFIED )
        {
            if ( expandCluster(*iter, clusterID) != FAILURE )
            {
                clusterID += 1;
            }
        }
    }

    return 0;
}

int DBSCAN::expandCluster(Point point, int clusterID)
{
    std::vector<int> clusterSeeds = calculateCluster(point);

    if ( clusterSeeds.size() < m_minPoints )
    {
        point.clusterID = NOISE;
        return FAILURE;
    }
    else
    {
        int index = 0, indexCorePoint = 0;
        std::vector<int>::iterator iterSeeds;
        for( iterSeeds = clusterSeeds.begin(); iterSeeds != clusterSeeds.end(); ++iterSeeds)
        {
            m_points.at(*iterSeeds).clusterID = clusterID;
            if (m_points.at(*iterSeeds).x == point.x && m_points.at(*iterSeeds).y == point.y && m_points.at(*iterSeeds).z == point.z  && m_points.at(*iterSeeds).t == point.t)
            {
                indexCorePoint = index;
            }
            ++index;
        }
        clusterSeeds.erase(clusterSeeds.begin()+indexCorePoint);

        for(std::vector<int>::size_type i = 0, n = clusterSeeds.size(); i < n; ++i )
        {
            std::vector<int> clusterNeighors = calculateCluster(m_points.at(clusterSeeds[i]));

            if ( clusterNeighors.size() >= m_minPoints )
            {
                std::vector<int>::iterator iterNeighors;
                for ( iterNeighors = clusterNeighors.begin(); iterNeighors != clusterNeighors.end(); ++iterNeighors )
                {
                    if ( m_points.at(*iterNeighors).clusterID == UNCLASSIFIED || m_points.at(*iterNeighors).clusterID == NOISE )
                    {
                        if ( m_points.at(*iterNeighors).clusterID == UNCLASSIFIED )
                        {
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

std::vector<int> DBSCAN::calculateCluster(Point point)
{
    int index = 0;
    std::vector<Point>::iterator iter;
    std::vector<int> clusterIndex;
    for( iter = m_points.begin(); iter != m_points.end(); ++iter)
    {
        if ( calculateDistance(point, *iter) <= m_epsilon && calculateTime(point, *iter) <= 1) // Hard code 1 ig
        {
            clusterIndex.push_back(index);
        }
        index++;
    }
    return clusterIndex;
}

inline double DBSCAN::calculateTime(const Point& pointCore, const Point& pointTarget )
{
    return pow(pointCore.t - pointTarget.t,2);
}

inline double DBSCAN::calculateDistance(const Point& pointCore, const Point& pointTarget )
{
    return pow(pointCore.x - pointTarget.x,2)+pow(pointCore.y - pointTarget.y,2)+pow(pointCore.z - pointTarget.z,2);
}
/*
void swap(int *arr, int i, int j){
    int temp = arr[i];
    arr[i] = arr[j];
    arr[j] = temp;
}

void sort_objects(int arr[N]){
    for (int i = 0; i < N - 1; i++){
        for (int j = 0; j < N - i - 1; j++){
// #pragma HLS PIPELINE II=2
	   if (arr[j] > arr[j + 1]){
                swap(arr, j, j + 1);
	    }
        }
    }
}
*/
/*
void clustering(data[N]){
//receives an array of structs
    for (int i=0; i<N; ++i){
	event = data[i];
	layer_id = event.LayerID;
	x = event.X;
	y = event.Y;
	z = event.Z;
	t = event.Time; //or charge
		


    }
}
*/

void printResults(std::vector<Point>& points, int num_points) {
    int i = 0;
    printf("Number of points: %u\n"
    " x     y     z     t     cluster_id    event_id \n"
    "-----------------------------\n"
    , num_points);
    while (i < num_points){
        printf("%5.2lf %5.2lf %5.2lf %5.21f: %d %d\n",
        points[i].x,
        points[i].y, points[i].z, points[i].t,
        points[i].clusterID);
	points[i].eventID;
        ++i;
    }
}



int main() {

    std::vector<Point> point;
    std::cout << "Checking if things compile" << std::endl;
    point = read_data();
    std::cout << "read_data() function works" << std::endl;

    std::cout << "Checking DBSCAN function" << std::endl;
    DBSCAN ds(MINIMUM_POINTS, EPSILON, point);
    ds.run();
    std::cout << "Clustering algorithm runs" << std::endl;
    printResults(ds.m_points, ds.getTotalPointSize());
    // printResults(ds.m_points, 20);
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
