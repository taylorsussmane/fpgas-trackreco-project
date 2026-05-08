#ifndef DBSCAN_H
#define DBSCAN_H

#include <vector>
#include <cmath>
#include <sstream>
#include<Eigen/Dense>

#define UNCLASSIFIED -1
#define CORE_POINT 1
#define BORDER_POINT 2
#define NOISE -2
#define SUCCESS 0
#define FAILURE -3
#define MINIMUM_POINTS 1     // minimum number of cluster
#define EPSILON (16*16) // (0.75*0.75)  // distance for clustering, metre^2

typedef struct Point_
{
    float x, y, z, t;  // X, Y, Z position & time
    int eventID, clusterID, layerID;  // event ID
}Point;

typedef struct trackerInput_
{
    Eigen::VectorXd x;  // X, Y, Z position & time
    Eigen::VectorXd dx, y, dy, z, dz, t, dt;
    Eigen::VectorXd eventID, clusterID;

}trackerInput;

class DBSCAN {
public:    
    DBSCAN(unsigned int minPts, float eps, std::vector<Point> points){
        m_minPoints = minPts;
        m_epsilon = eps;
        m_points = points;
        m_pointSize = points.size();
    }
    ~DBSCAN(){}

    int run();
    std::vector<int> calculateCluster(Point point);
    int expandCluster(Point point, int clusterID);
    inline double calculateDistance(const Point& pointCore, const Point& pointTarget);
    inline double calculateTime(const Point& pointCore, const Point& pointTarget);

    int getTotalPointSize() {return m_pointSize;}
    int getMinimumClusterSize() {return m_minPoints;}
    int getEpsilonSize() {return m_epsilon;}
    
public:
    std::vector<Point> m_points;
    
private:    
    unsigned int m_pointSize;
    unsigned int m_minPoints;
    float m_epsilon;
};

#endif // DBSCAN_H
