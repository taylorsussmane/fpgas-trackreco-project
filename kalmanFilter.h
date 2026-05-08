#include <Eigen/Dense>

class KalmanFilter {

	public:
		KalmanFilter(
			double dt,
			const Eigen::MatrixXd& A, //system dynamics matrix
			const Eigen::MatrixXd& C, //output matrix
			const Eigen::MatrixXd& Q, //process noise covariance
			const Eigen::MatrixXd& R, //measurement noise covariance
			const Eigen::MatrixXd& P //estimate error covariance
		);
		
		KalmanFilter();
		void init();
		void init(double t0, const Eigen::VectorXd& x0);
		void update(const Eigen::VectorXd& y);
		void update(const Eigen::VectorXd& y, double dt, const Eigen::MatrixXd A);
		Eigen::VectorXd state(){return x_hat;};
		double time(){return t;};

	private:
		Eigen::MatrixXd A, C, Q, R, P, K, P0;
		int m, n; //system dimensions
		double t0, t; //initial and current time
		double dt; //discrete time step
		bool initialized;
		Eigen::MatrixXd I; //identity
		Eigen::VectorXd x_hat, x_hat_new; //estimated states
};
