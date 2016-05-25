#include <quotek/kalman.hpp>

// Standard includes
#include <iostream>
#include <iomanip>
#include <cmath>
#include <ctime>
#include <utility>
#include <vector>

#include <Eigen/Core>

#define COL 10


using namespace eigenkf;


typedef std::pair<Eigen::Vector2d, Eigen::Vector2d> StatePair;

double noise(double noiseAmplitude = 3.0) {
  return ((std::rand() % 100) / 50.0 - 1.0) * noiseAmplitude;
}

std::vector<StatePair> generateLineData(const double dt = 0.5, const double maxTime = 50.0, double noiseAmplitude = 3.0) {
  std::vector<StatePair> ret;
  for (double t = 0; t < maxTime; t += dt) {
    Eigen::Vector2d err;
    err[0] = noise(noiseAmplitude);
    err[1] = noise(noiseAmplitude);
    Eigen::Vector2d pos(Eigen::Vector2d::Constant(t));
    Eigen::Vector2d measurement(pos + err);
    ret.push_back(StatePair(pos, measurement));
  }
  return ret;
}

std::vector<StatePair> generateSineData(const double dt = 0.5, const double maxTime = 50.0, double noiseAmplitude = 3.0, double maxHeight = 10.0) {
  std::vector<StatePair> ret;
  const double timeScale = 3.141592653589 / maxTime;
  for (double t = 0; t < maxTime; t += dt) {
    Eigen::Vector2d err;
    err[0] = noise(noiseAmplitude);
    err[1] = noise(noiseAmplitude);

    Eigen::Vector2d pos;
    pos[0] = t;
    pos[1] = std::sin(t * timeScale) * maxHeight;

    Eigen::Vector2d measurement(pos + err);
    ret.push_back(StatePair(pos, measurement));
  }
  return ret;
}


int main(int /*argc*/, char * /*argv*/[]) {
  std::srand(std::time(NULL));

  /// We want a simple 2d state
  typedef SimpleState<2> state_t;

  /// Our process model is the simplest possible: doesn't change the mean
  typedef ConstantProcess<2, state_t> process_t;

  /// Create a kalman filter instance with our chosen state and process types
  KalmanFilter<state_t, process_t> kf;

  /// Set our process model's variance
  kf.processModel.sigma = state_t::VecState::Constant(6.5);

  double dt = 0.5;
  double sumSquaredError = 0;
  
  const double measurementVariance = 9; //NOISE_AMPLITUDE / 2.0;

  /// for sine curve
  std::vector<StatePair> data = generateSineData(dt);
  
  /// CSV header row
  //std::cout << "actual x,actual y,measurement x,measurement y,filtered x,filtered y,squared error" << std::endl;
  
  double t = 0;
  for (unsigned int i = 0; i < data.size(); ++i) {
    /// Predict step: Update Kalman filter by predicting ahead by dt
    kf.predict(dt);

    /// "take a measurement" - in this case, noisify the actual measurement
    Eigen::Vector2d pos = data[i].first;
    Eigen::Vector2d m = data[i].second;
    AbsoluteMeasurement<state_t> meas;
    meas.measurement = m;
    meas.covariance = Eigen::Vector2d::Constant(measurementVariance).asDiagonal();

    /// Correct step: incorporate information from measurement into KF's state
    kf.correct(meas);

    /// Output info for csv
    double squaredError = (pos[0] - kf.state.x[0]) * (pos[0] - kf.state.x[0]);
    sumSquaredError += squaredError;
    
    /*std::cout << pos[0] << "," << pos[1] << ",";
    std::cout << meas.measurement[0] << "," << meas.measurement[1] << ",";
    std::cout << kf.state.x[0] << "," << kf.state.x[1] << ",";
    std::cout << squaredError;
    std::cout << std::endl;
    */
    
    t += dt;

  }

  //std::cerr << "Sum squared error: " << sumSquaredError << std::endl;
  assert( sumSquaredError < 500 );

  return 0;

}